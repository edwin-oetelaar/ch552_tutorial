#!/usr/bin/env python3

import argparse
from pathlib import Path
import struct
import random
import sys


def random_byte():
    return random.randbytes(1)


# typeall.wcfg
chip_id_map = {
    "CH561": 97,
    "CH563": 99,
    "CH565": 101,
    "CH566": 102,
    "CH567": 103,
    "CH568": 104,
    "CH569": 105,
    "CH551": 81,
    "CH552": 82,
    "CH554": 84,
    "CH555": 85,
    "CH556": 86,
    "CH557": 87,
    "CH558": 88,
    "CH559": 89,
    "CH544": 68,
    "CH545": 69,
    "CH546": 70,
    "CH547": 71,
    "CH548": 72,
    "CH549": 73,
    "CH571": 113,
    "CH573": 115,
    "CH577": 119,
    "CH578": 120,
    "CH579": 121,
    "CH32F103": 50,
    "CH32V103": 50,
    "CH581": 129,
    "CH582": 130,
    "CH583": 131,
    "CH32V303": 48,
    "CH32V305": 80,
    "CH32V307": 112,
    "CH32F203": 48,
    "CH32F205": 80,
    "CH32F207": 112,
    "CH32F208": 128
}


def guess_chip_type(chip_id):
    return ' | '.join([k for k, v in chip_id_map.items() if v == chip_id]) or 'unknown'


def hexit(out):
    return bytearray(out).hex(' ', 0)


def debug(out, prefix=":", ):
    for ch in range(len(out) // 16 + 1):
        chunk = out[ch * 16:ch * 16 + 16]
        if chunk:
            print(prefix, "%02x|" % (ch*16), bytearray(chunk).hex(' ', 1), end=' | ')
            for c in chunk:
                if c >= 32 and c <= 126:
                    print(chr(c), end='')
                else:
                    print('.', end='')
            print('')
    if len(chunk) != 16:
        print(prefix, "%02x|" % len(out))


def parse_usb_id(value):
    try:
        return int(value, 0)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"invalid USB id: {value}") from exc


class ISPFlasher(object):
    def __init__(self, vid=0x4348, pid=0x55e0, verbose=True) -> None:
        self.verbose = verbose

        try:
            import usb.core
        except ModuleNotFoundError as exc:
            raise RuntimeError(
                "Missing Python dependency 'pyusb'. Install it with: "
                "python3 -m pip install pyusb  (or: sudo apt install python3-usb)"
            ) from exc

        self.dev = dev = usb.core.find(idVendor=vid, idProduct=pid)
        if dev is None:
            raise RuntimeError(
                f"No WCH ISP USB device found at VID:PID {vid:04x}:{pid:04x}. "
                "Put the chip in bootloader/ISP mode and check USB permissions."
            )

        # set the active configuration. With no arguments, the first
        # configuration will be the active one
        dev.set_configuration()

        # get an endpoint instance
        cfg = dev.get_active_configuration()
        # First configuration, first interface
        intf = cfg[(0, 0)]

        # 0,0: vendoer specific
        if self.verbose:
            print(intf)

        # self.init()

    def init(self):
        cmd_read_chip = [0xA1, 0x12, 0x00, 0x00, 0x11, 0x4D, 0x43, 0x55, 0x20, 0x49,
                         0x53, 0x50, 0x20, 0x26, 0x20, 0x57, 0x43, 0x48, 0x2e, 0x43, 0x4e]
        rx = self.xcmd(cmd_read_chip)

        self.chip_id = chip_id = rx[4]
        print("chip id:", hex(chip_id), chip_id,  guess_chip_type(chip_id))

        # Read the configuration (unique ID, bootloader version, config bits)
        cmd_read_chip_conf = [0xa7, 0x02, 0x00, 0x1f, 0x00]
        rx = self.xcmd(cmd_read_chip_conf)

        # TODO, write via a8
        self.reg_CONF = rx[6:18]
        print("CONF:", hexit(self.reg_CONF))

        assert rx[6] ^ rx[7] == 0xff, "check RDPR and nRDPR"
        reg_RDPR = rx[6]

        if reg_RDPR == 0xa5:
            self.read_protect = False
            print("Read Protection: no")
        else:
            self.read_protect = True
            print("Read Protection: yes")

        assert rx[8] ^ rx[9] == 0xff, "check USER and nUSER"
        reg_USER = rx[8]
        print("IWDG_SW", bool(reg_USER & 0b1))
        print("STOP_RST", bool(reg_USER & 0b10))
        print("STANDY_RST", bool(reg_USER & 0b100))
        print("SBD_MODE", bool(reg_USER & 0b1000))
        print("USBD_PU", bool(reg_USER & 0b10000))
        print("POR_CTR", bool(reg_USER & 0b100000))

        print("DATA0", hex(rx[10]), "\nDATA1", hex(rx[12]))

        WRP = rx[14:18]
        print("WRP", hexit(WRP))

        bootloader_verion = '.'.join(map(str, rx[18:22]))
        print("bootloader version:", bootloader_verion)

        unique_id = rx[22:30]
        self.device_uid = unique_id
        print("Device UID:", bytearray(unique_id).hex('-', 1))

    def prepare_xor_key(self):
        s = sum(self.device_uid) & 0xff
        xor_key = [s] * 8
        xor_key[-1] += self.chip_id
        xor_key[-1] &= 0xff

        print("xor key:", hexit(xor_key))
        self.xor_key = xor_key

    def write_flash(self, data):
        cmd_set_remote_encryption_key = [0xA3, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
        rx = self.xcmd(cmd_set_remote_encryption_key)

        checksum = rx[4]
        assert checksum == sum(self.xor_key) & 0xff, "xor key checksum check"

        CHUNK = 56
        chunks = len(data) // CHUNK + 1
        offset = 0
        for ch in range(chunks):
            chunk = data[ch * CHUNK: (ch + 1) * CHUNK]
            if len(chunk) == 0:
                break
            self.write_chunk(offset, chunk)
            offset += len(chunk)

        # last empty write
        self.write_chunk(offset, b'')

    def write_chunk(self, offset, chunk):
        payload = bytearray(chunk)
        for i in range(len(payload)):
            payload[i] ^= self.xor_key[i % 8]

        # NOTE: 00 here should be a random byte
        cmd = b'\xa5' + struct.pack('<HL', len(payload)+4+1, offset) + b'\x00' + payload
        self.xcmd(cmd)

    def verify_chunk(self, offset, chunk):
        payload = bytearray(chunk)
        for i in range(len(payload)):
            payload[i] ^= self.xor_key[i % 8]
            # pass

        cmd = b'\xa6' + struct.pack('<HL', len(payload)+4, offset) + payload
        self.xcmd(cmd)

    def unprotect(self):
        if not self.read_protect:
            return

        raise RuntimeError("unimpl")

    def erase_code(self):
        cmd_erase_code = bytes.fromhex("a4 04 00 08 00 00 00".replace(' ', ''))
        rx = self.xcmd(cmd_erase_code)

    def reset(self):
        cmd_reset = bytes.fromhex("a2 01 00 01".replace(' ', ''))
        rx = self.xcmd(cmd_reset)

    def write_conf(self):
        # cmd = a8
        pass

    def xcmd(self, cmd):
        self.send(cmd)
        return self.recv()

    # BULK out
    def send(self, cmd):
        if self.verbose:
            debug(cmd, prefix="==>")
        return self.dev.write(0x2, cmd)

    # BULK in
    def recv(self, n=64):
        rx = self.dev.read(0x82, n)
        if self.verbose:
            debug(rx, prefix="<==")
        return rx


def build_arg_parser():
    parser = argparse.ArgumentParser(
        description="Flash a binary image to a WCH CH55x device in USB ISP bootloader mode.",
    )
    parser.add_argument(
        "bin_file",
        type=Path,
        help="Binary image to flash, for example build/ch552_blink.bin",
    )
    parser.add_argument(
        "--vid",
        type=parse_usb_id,
        default=0x4348,
        help="USB vendor ID of the ISP device (default: 0x4348)",
    )
    parser.add_argument(
        "--pid",
        type=parse_usb_id,
        default=0x55e0,
        help="USB product ID of the ISP device (default: 0x55e0)",
    )
    parser.add_argument(
        "--no-reset",
        action="store_true",
        help="Do not send the reset command after flashing",
    )
    parser.add_argument(
        "--quiet",
        action="store_true",
        help="Suppress USB packet debug output",
    )
    return parser


def main(argv=None):
    parser = build_arg_parser()
    args = parser.parse_args(argv)

    if not args.bin_file.is_file():
        parser.error(f"binary file does not exist: {args.bin_file}")

    try:
        raw = args.bin_file.read_bytes()
        if len(raw) == 0:
            raise RuntimeError(f"binary file is empty: {args.bin_file}")

        flasher = ISPFlasher(vid=args.vid, pid=args.pid, verbose=not args.quiet)
        flasher.init()
        flasher.erase_code()
        flasher.prepare_xor_key()
        flasher.write_flash(raw)

        if not args.no_reset:
            flasher.reset()

        print(f"Flashed {len(raw)} bytes from {args.bin_file}")
        return 0
    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
