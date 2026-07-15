# CH552 Computer Architecture Learning Project

**Educational CH552 project for learning 8051 computer architecture, low-level hardware bring-up, embedded C, finite state machines, modular driver design, testing, and software engineering from reset vector to complete application.**

## Overview

This repository is an educational project for learning computer architecture, embedded systems, and low-level software engineering using the **CH552** microcontroller.

The CH552 is a small and resource-constrained microcontroller based on the 8051/8052 architecture, with additional peripherals such as USB, timers, GPIO, UART, SPI, and ADC functionality.

Its limited memory and simple architecture make it highly suitable for education. Students cannot rely on large frameworks or excessive abstraction. They must understand:

* how the processor starts;
* how memory is organized;
* how registers control hardware;
* how interrupts work;
* how code reaches `main`;
* how a main loop is structured;
* how peripherals are initialized;
* how embedded software is built step by step.

The project uses:

* SDCC;
* Make;
* C;
* Git;
* datasheets and reference manuals as primary documentation.

The goal is not merely to make the board do something. The goal is to understand why it works.

---

# Educational Goal

This project is part of a broader learning path in embedded software engineering.

Students often start by using frameworks and libraries that hide the underlying system. That is useful for rapid prototyping, but it can also make it difficult to understand what actually happens inside a microcontroller.

This project takes the opposite approach.

We start from the lowest useful level and gradually build a complete embedded application:

1. processor reset and boot vector;
2. startup code;
3. memory organization;
4. special function registers;
5. clock configuration;
6. GPIO bring-up;
7. timers and interrupts;
8. communication peripherals;
9. finite state machines;
10. non-blocking main-loop architecture;
11. modular driver design;
12. testing and quality assurance;
13. complete application integration.

Each step should remain small, understandable, testable, and reproducible.

---

# Why the CH552?

The CH552 is deliberately small.

That is an advantage.

On larger microcontrollers, it is easy to add more libraries, more threads, more memory, and more abstraction layers. On the CH552, resources are limited and architecture decisions become visible.

Students must consider:

* code size;
* RAM usage;
* stack usage;
* register configuration;
* interrupt latency;
* blocking versus non-blocking code;
* direct hardware access;
* compile-time configuration;
* modularity without unnecessary overhead.

This makes the CH552 a practical platform for learning both computer architecture and embedded software engineering.

---

# Learning Outcomes

After completing the project, a student should be able to:

* explain the basic 8051/8052 architecture;
* describe what happens after a processor reset;
* locate and explain the reset vector;
* understand code, data, and special-function-register address spaces;
* configure hardware directly through registers;
* read and use a microcontroller datasheet;
* write a minimal startup sequence;
* create a reliable hardware bring-up procedure;
* implement a structured main loop;
* distinguish blocking and non-blocking behavior;
* use interrupts safely;
* create simple device drivers;
* separate hardware-specific code from application logic;
* design software using finite state machines;
* use Git for incremental development;
* use automated builds and tests where possible;
* review AI-generated code instead of trusting it blindly.

---

# Engineering Principles

This project is not only about programming. It is about engineering.

The following principles guide the repository.

## Understand before abstracting

Do not introduce an abstraction before the underlying mechanism is understood.

## Start small

Each step should introduce one concept or one hardware feature.

## Make changes traceable

Use small Git commits with clear commit messages.

## Keep modules focused

Each module should have one clear responsibility.

## Prefer explicit behavior

Register configuration, timing assumptions, and state transitions should be visible in the code.

## Avoid hidden dependencies

Modules should not silently depend on global state.

## Design for testing

Hardware-independent logic should be testable without the target board whenever practical.

## Fail early

Invalid configuration and unsupported assumptions should be detected as early as possible.

## Use AI as a tool

AI may assist with:

* code generation;
* test generation;
* documentation;
* register summaries;
* review suggestions.

The student remains responsible for:

* correctness;
* architecture;
* hardware behavior;
* safety;
* verification;
* final acceptance.

Generated code is not evidence that the system works.

---

# Development Environment

## Required tools

* SDCC
* Make
* Git
* `wchisp` for flashing over the CH552 USB bootloader
* CH552-compatible programmer
* CH552 development board or custom hardware

## Recommended tools

* logic analyzer;
* oscilloscope;
* serial adapter;
* USB protocol analyzer;
* static analysis tools;
* unit-test framework for host-side logic.

---

# Building the Project

Clone the repository:

```bash
git clone <repository-url>
cd <repository-directory>
```

Build the firmware:

```bash
make
```

This creates `build/ch552_blink.hex` and keeps SDCC intermediate files in `build/`.

Upload the firmware with `wchisp`:

```bash
make flash
```

Clean the build:

```bash
make clean
```

The exact upload procedure depends on the programmer and board configuration.

---

# Repository Structure

A possible project structure is:

```text
.
├── docs/
│   ├── afbeeldingen/
│   ├── board/
│   ├── datasheets/
│   ├── markdown/
│   ├── pdf/
│   └── scripts/
├── examples/
│   ├── 00-reset-vector/
│   ├── 01-startup/
│   ├── 02-gpio-bringup/
│   ├── 03-timer/
│   ├── 04-interrupts/
│   ├── 05-uart/
│   ├── 06-usb/
│   ├── 07-fsm/
│   └── 08-application/
├── include/
├── src/
├── build/          # generated by make, not committed
├── Makefile
├── CHANGELOG.md
└── README.md
```

The numbered examples represent the intended learning order.

---

# Learning Path

## 0. Processor Reset and Boot Vector

The first exercise starts at the reset vector.

Students investigate:

* where the processor begins execution;
* what address is used after reset;
* how control reaches the startup code;
* how interrupt vectors are organized;
* what the linker and compiler generate automatically.

Questions to answer:

* What is the reset address?
* What instruction is placed there?
* Where is `main` located?
* What happens before `main` is called?
* Which memory sections are initialized?

---

## 1. Startup Code

The next step is understanding the runtime startup sequence.

Topics include:

* stack initialization;
* global variable initialization;
* zero-initialized data;
* initialized data;
* code memory;
* internal RAM;
* special function registers;
* compiler-generated startup code.

Students should inspect generated assembly and map files where possible.

---

## 2. Hardware Bring-Up

Hardware bring-up should be performed in small, observable steps.

A recommended sequence is:

1. verify programming;
2. confirm reset behavior;
3. configure the system clock;
4. configure one GPIO output;
5. toggle a pin;
6. verify timing with a logic analyzer or oscilloscope;
7. add one input;
8. verify register values.

A blinking LED is not the final goal. It is evidence that several architectural assumptions are correct.

---

## 3. Registers and Peripherals

Students should learn to work directly from the datasheet.

For each peripheral:

1. identify the relevant registers;
2. identify reset values;
3. identify writable and read-only bits;
4. define configuration requirements;
5. implement the smallest working configuration;
6. verify the result on hardware;
7. document assumptions.

Do not copy register values without explaining them.

Avoid unexplained magic numbers.

Prefer named masks and constants.

Example:

```c
#define TIMER0_ENABLE_MASK 0x10u
```

instead of:

```c
REGISTER |= 0x10;
```

---

## 4. Main-Loop Architecture

The initial application uses a single main loop.

A minimal structure may look like:

```c
int main(void)
{
    hardware_init();

    while (1)
    {
        input_update();
        application_update();
        output_update();
    }
}
```

Students should understand:

* why initialization happens once;
* why repeated work belongs in the loop;
* why long delays are dangerous;
* how blocking code affects responsiveness;
* how loop timing can be measured;
* how work can be divided into modules.

---

## 5. Blocking and Non-Blocking Design

Blocking code is easy to write, but it prevents other work from progressing.

Example of blocking behavior:

```c
delay_ms(1000);
```

During this delay, the application may be unable to:

* process input;
* handle communication;
* update control logic;
* detect faults;
* meet timing requirements.

Students should gradually replace blocking behavior with:

* timer-based scheduling;
* state machines;
* event flags;
* periodic updates;
* non-blocking drivers.

---

## 6. Finite State Machines

Finite state machines are used to structure behavior.

A state machine contains:

* states;
* events;
* transitions;
* actions;
* guards;
* timeout behavior.

Possible exercises:

* button debouncing;
* LED pattern controller;
* UART command parser;
* USB device state;
* startup sequence;
* fault recovery;
* simple motor controller.

State machines should be documented before implementation.

---

## 7. Interrupts

Interrupts are introduced only after the main-loop model is understood.

Students should learn:

* interrupt vectors;
* interrupt enable registers;
* priority;
* shared data;
* atomic access;
* interrupt latency;
* critical sections;
* why interrupt handlers should remain short.

Recommended rule:

> Capture the event in the interrupt handler and process the event in the main context.

Avoid complex application logic inside interrupt handlers.

---

## 8. Device Drivers

Drivers should expose a small and clear interface.

Example:

```c
typedef enum
{
    DRIVER_OK = 0,
    DRIVER_ERROR,
    DRIVER_NOT_READY
} driver_result_t;

driver_result_t uart_init(void);
driver_result_t uart_write(const uint8_t *data, uint8_t length);
driver_result_t uart_read(uint8_t *data, uint8_t length);
```

Drivers should be separated from application logic.

A driver should not decide what the application means by the data.

The application should not directly manipulate peripheral registers once a driver boundary has been introduced.

---

## 9. Modular Software Design

As the project grows, software is separated into layers.

A possible architecture is:

```text
application
    ↓
services
    ↓
device drivers
    ↓
hardware access
    ↓
CH552 registers
```

Possible modules include:

* clock;
* GPIO;
* timer;
* UART;
* USB;
* scheduler;
* state machine;
* application;
* diagnostics.

Each module should have:

* a clear purpose;
* a small public API;
* hidden implementation details;
* explicit dependencies;
* defined error behavior.

---

## 10. Testing

Not all embedded code can be tested on a development computer, but a large part of the logic can.

Suitable host-side test targets include:

* state machines;
* parsers;
* conversion functions;
* protocol logic;
* scheduling logic;
* validation functions;
* fault handling.

Hardware access can be replaced by:

* fakes;
* stubs;
* mocks;
* simulated registers.

Tests should use a clear structure:

1. Arrange;
2. Act;
3. Assert.

The project should include regression tests for previously discovered bugs.

---

## 11. Git Workflow

Development should proceed incrementally.

Recommended workflow:

1. define one small objective;
2. create or update a test;
3. implement the smallest change;
4. build the project;
5. run tests;
6. verify on hardware where required;
7. review the change;
8. commit with a clear message.

Example commit messages:

```text
Add minimal reset-vector example
Document CH552 memory spaces
Add GPIO output bring-up
Replace blocking LED delay with timer state
Add UART parser tests
```

Avoid commits such as:

```text
updates
changes
fixed stuff
final version
```

---

# Datasheets and Documentation

The `docs/` directory should contain or reference:

* CH552 datasheet;
* CH552 reference manual;
* 8051/8052 architecture documentation;
* SDCC documentation;
* Makefile notes;
* programmer documentation;
* board schematics;
* pinout information;
* register notes;
* errata where available.

Always verify whether redistribution of a document is permitted before storing a copy in the repository.

When redistribution is unclear, store a link and document the exact version used.

---

# Suggested Exercises

Possible exercises include:

1. Locate the reset vector.
2. Inspect generated assembly.
3. Toggle a GPIO without a framework.
4. Measure the toggle frequency.
5. Configure a timer.
6. Generate a periodic interrupt.
7. Build a non-blocking LED controller.
8. Implement button debouncing as an FSM.
9. Build a UART driver.
10. Implement a command parser.
11. Add a watchdog or fault timeout.
12. Separate driver and application layers.
13. Add host-side tests.
14. Build a complete USB or control application.
15. Review AI-generated driver code against the datasheet.

---

# Definition of Done

A feature is not complete because it compiles.

A feature is complete when:

* the requirement is clear;
* the design is documented;
* the code builds without unexpected warnings;
* relevant tests pass;
* hardware behavior has been verified;
* register assumptions are documented;
* failure behavior is known;
* the change is committed;
* the repository remains reproducible.

---

# Project Philosophy

This project follows a simple progression:

```text
understand
    ↓
design
    ↓
implement
    ↓
test
    ↓
verify
    ↓
integrate
```

The CH552 is small enough that students can see the complete system.

That is the purpose of the project.

The objective is not to hide the hardware behind increasingly large abstractions. The objective is to understand the hardware well enough to create abstractions that are justified, minimal, testable, and reliable.

---

# Status

This repository is under active development as an educational project.

The planned progression is:

* reset vector;
* startup;
* memory model;
* clock;
* GPIO;
* timers;
* interrupts;
* UART;
* USB;
* finite state machines;
* modular drivers;
* testing;
* complete application.

Contributions should preserve the educational sequence and keep each step understandable.

---

# License

* MIT;

---

# Author

Edwin van den Oetelaar

Fontys University of Applied Sciences
High Tech Embedded Software
