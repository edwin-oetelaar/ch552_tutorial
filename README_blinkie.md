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

### Flash using Windows

The official **WCHISPTool** can also be used.

1. Select the 8051 device family.
2. Select the CH552.
3. Open `build/ch552_blink.hex`.
4. Press **Download**.

---

## 5. Educational Concepts

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
