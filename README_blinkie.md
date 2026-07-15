# CH552 LED Blinker Project

## Overview

This is a small educational project that blinks an LED using a **WCH CH552** microcontroller.

The project is intended for teaching basic concepts in:

* computer architecture;
* embedded C;
* special function registers;
* bit-addressable I/O;
* compiler-generated machine code;
* Intel HEX files;
* low-level hardware bring-up.

The CH552 is based on the MCS-51/8051 architecture and has very limited resources. This makes it a useful platform for learning how a microcontroller works without hiding the hardware behind large frameworks.

This project is intended as the first step before moving to a small modern RISC-V microcontroller such as the **WCH CH32V003F4P6**. The CH32V003 is also very inexpensive, but it has a 32-bit RISC-V core, a different startup model, a vector table, and more modern peripherals. Understanding the CH552 first makes those differences easier to see.

The course does not use the classic Arduino Uno / AVR ATmega328P as the main route. That platform is useful, but there are already many tutorials for it and the Arduino framework hides much of the architecture. Students can explore AVR/Arduino independently if they want extra background.

---

## 1. Compiler

The project uses the **SDCC — Small Device C Compiler**.

SDCC is an open-source C compiler for small 8-bit microcontrollers, including the MCS-51/8051 architecture.

### Linux or WSL

```bash
sudo apt update
sudo apt install sdcc
```

### Windows

Download SDCC from:

[https://sdcc.sourceforge.net/](https://sdcc.sourceforge.net/)

After installation, add the SDCC `bin` directory to the system `PATH`.

---

## 2. Project Files

* `src/main.c`
  Contains the LED blinker application and a simple software delay.

* `include/ch552.h`
  Contains the CH552 Special Function Register definitions.

* `Makefile`
  Contains the build instructions.

* `docs/datasheets/ch552_datasheet_en.pdf`
  CH552 datasheet containing the hardware description and register map.

* `docs/markdown/hex_format.md`
  Explains the Intel HEX format and how compiled code can be related to machine code and assembly.

---

## 3. Build

Open a terminal in the project directory and run:

```bash
make
```

This creates:

```text
build/ch552_blink.hex
```

`build/ch552_blink.hex` is an Intel HEX file containing the program that will be written to the microcontroller flash memory.

To remove generated build files:

```bash
make clean
```

---

## 4. Flashing the CH552

The CH552 contains a built-in **USB ISP bootloader**. An external programmer such as a J-Link or ST-Link is therefore not required.

### Enter Bootloader Mode

The exact procedure depends on the development board.

A typical procedure is:

1. Disconnect the USB cable.
2. Hold the `BOOT` or `PROG` button.
3. Connect the USB cable.
4. Release the button.

Some boards use a specific pin instead of a button. Always check the schematic or documentation of the board before connecting a pin to power or ground.

### Flash from the Command Line

This project can be flashed using `wchisp`.

Install it with:

```bash
cargo install wchisp
```

Flash the firmware with:

```bash
make flash
```

This runs `wchisp flash build/ch552_blink.hex`.

When flashing succeeds, output should look similar to this:

```text
10:45:23 [INFO] Opening USB device #0
10:45:23 [INFO] Chip: CH552[0x5211] (Code Flash: 14KiB, Data EEPROM: 128 Bytes)
10:45:23 [INFO] Chip UID: B7-6E-EE-BD-00-00-00-00
10:45:23 [INFO] BTVER(bootloader ver): 02.50
10:45:23 [INFO] Read /home/fontys/ch552_blink/build/ch552_blink.hex as IntelHex format
10:45:23 [INFO] Firmware size: 1024
10:45:23 [INFO] Erasing...
10:45:24 [INFO] Erase done
10:45:24 [INFO] Writing to code flash...
10:45:24 [INFO] Code flash 1024 bytes written
10:45:25 [INFO] Verifying...
10:45:25 [INFO] Verify OK
10:45:25 [INFO] Now reset device and skip any communication errors
```

This is the most important milestone in the project. At this point the whole
chain has been proven: SDCC built the program, `packihx` produced a valid Intel
HEX file, Linux can access the USB bootloader, `wchisp` can erase/write/verify
flash, and the CH552 runs the new firmware. If the LED blinks, you know the
board, bootloader, build system and software are all in a good state. From here
you can start working on the actual application with confidence.

### USB permissions on Linux

If the board is visible with `lsusb` but flashing fails because of permissions, install the udev rule included in this repository:

```bash
sudo cp docs/udev/99-ch55x.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Then unplug and reconnect the board.

To verify that the bootloader is visible, put the board in ISP/bootloader mode
and run:

```bash
wchisp -u info
```

Expected output looks similar to this:

```text
10:37:15 [INFO] Opening USB device #0
10:37:15 [INFO] Chip: CH552[0x5211] (Code Flash: 14KiB, Data EEPROM: 128 Bytes)
10:37:15 [INFO] Chip UID: B7-6E-EE-BD-00-00-00-00
10:37:15 [INFO] BTVER(bootloader ver): 02.50
10:37:15 [INFO] Current config registers: ffffffffffffffffff52808000020500b76eeebd00000000
REVERSED: 0xFFFFFFFF
WPROTECT: 0xFFFFFFFF
  [0:0]   NO_KEY_SERIAL_DOWNLOAD 0x1 (0b1)
    `- Enable
  [1:1]   DOWNLOAD_CFG 0x1 (0b1)
    `- P4.6 / P15 / P3.6(Default set)
GLOBAL_CFG: 0x808052FF
  [15:15] CODE_PROTECT 0x0 (0b0)
    `- Forbid code & data protection
  [14:14] NO_BOOT_LOAD 0x1 (0b1)
    `- Boot from 0xf400 Bootloader
  [13:13] EN_LONG_RESET 0x0 (0b0)
    `- Short reset
  [12:12] XT_OSC_STRONG 0x1 (0b1)
    `- Enhanced
  [11:11] EN_P5.7_RESET 0x0 (0b0)
    `- Forbid
  [10:10] EN_P0_PULLUP 0x0 (0b0)
    `- Forbid
  [9:8]   RESERVED 0x2 (0b10)
    `- Default
  [7:0]   RESERVED 0xFF (0b11111111)
    `- Default
```

The exact UID and timestamps will differ. The important parts are that the
device opens, the chip is detected as `CH552`, and the bootloader version is
reported.

The rule covers the CH55xduino USB ID:

```text
1209:c550 Generic CH55xduino
```

When the firmware runs, Linux may also expose the board as a serial device:

```text
ttyACM0: USB ACM device
```

The udev rule also covers that `ttyACM*` interface and creates a stable symlink:

```text
/dev/ch55xduino
```

It also covers the WCH ISP bootloader ID used by some tools:

```text
4348:55e0
```

Temporary workaround for the currently connected device only:

```bash
sudo chmod 666 /dev/bus/usb/001/006
```

Replace `001/006` with the bus and device numbers shown by `lsusb`.

### Flash using Windows

The official **WCHISPTool** can also be used.

1. Select the 8051 device family.
2. Select the CH552.
3. Open `build/ch552_blink.hex`.
4. Press **Download**.

---

## 5. Educational Concepts

The concepts in this first CH552 lab will return in the later CH32V003/RISC-V introduction. The second chip should follow the same order: minimal startup, reset entry, vector table, basic GPIO, then deeper work with modern peripherals.

### Special Function Registers

The 8051 architecture uses a dedicated Special Function Register space.

The SFR address range is:

```text
0x80 to 0xFF
```

Registers such as GPIO ports, timers and interrupt controllers are accessed through this special address space.

---

### Bit-Addressable I/O

Some 8051 registers are bit-addressable.

For example, individual GPIO pins can be controlled directly instead of first reading, modifying and writing an entire port register.

This allows code such as:

```c
LED_PIN = 1;
```

The compiler can translate this into an instruction that changes one specific bit.

---

### 8-Bit Architecture

The CH552 CPU is based on an 8-bit architecture.

Registers such as accumulator `A` and register `B` are 8 bits wide.

Operations on larger data types require multiple machine instructions. For example, a 16-bit `unsigned int` operation may require several 8-bit operations.

This demonstrates why data width affects:

* execution time;
* code size;
* memory usage;
* performance.

---

### Assembly and the NOP Instruction

The following statement inserts a `NOP` instruction:

```c
__asm__("nop");
```

`NOP` means **No Operation**.

The processor executes the instruction without changing registers or memory. It can be used to study instruction timing and generated assembly.

---

## 6. Learning Goal

The goal of this project is not only to blink an LED.

Students should be able to explain:

* how execution starts after reset;
* how the compiler translates C into machine code;
* how GPIO is controlled through registers;
* why data width matters;
* how software delays work;
* how the Intel HEX file represents program memory;
* how source code relates to assembly instructions.

The main principle is:

> Do not only make it work. Understand why it works.

---

## Author

Edwin van den Oetelaar

Fontys University of Applied Sciences
High Tech Embedded Software
