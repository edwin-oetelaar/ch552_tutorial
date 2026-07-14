#ifndef CH552_H
#define CH552_H

/* Special Function Register (SFR) definitions for WCH CH552 (SDCC compiler syntax) */

/* Port 1 registers */
__sfr __at (0x90) P1;         /* Port 1 Input/Output register */
__sfr __at (0x92) P1_MOD_OC;  /* Port 1 Output Mode: 0 = push-pull/input, 1 = open-drain */
__sfr __at (0x93) P1_DIR_PU;  /* Port 1 Direction & Pull-Up: 0 = input/open-drain, 1 = output/pull-up */

/* Port 3 registers */
__sfr __at (0xB0) P3;         /* Port 3 Input/Output register */
__sfr __at (0x96) P3_MOD_OC;  /* Port 3 Output Mode: 0 = push-pull/input, 1 = open-drain */
__sfr __at (0x97) P3_DIR_PU;  /* Port 3 Direction & Pull-Up: 0 = input/open-drain, 1 = output/pull-up */

/* System clock configuration registers */
__sfr __at (0x8E) CLOCK_CFG;  /* Clock configuration register */
__sfr __at (0x8F) SAFE_MOD;   /* Safe mode register for unlocking system settings */

/* Helper macros for Bit-addressable registers */
/* In SDCC, __sbit is used to define individual bits of bit-addressable SFRs (like P1 and P3) */
__sbit __at (0x90 + 0) P1_0;
__sbit __at (0x90 + 1) P1_1;
__sbit __at (0x90 + 2) P1_2;
__sbit __at (0x90 + 3) P1_3;
__sbit __at (0x90 + 4) P1_4;
__sbit __at (0x90 + 5) P1_5;
__sbit __at (0x90 + 6) P1_6;
__sbit __at (0x90 + 7) P1_7;

__sbit __at (0xB0 + 0) P3_0;
__sbit __at (0xB0 + 1) P3_1;
__sbit __at (0xB0 + 2) P3_2;
__sbit __at (0xB0 + 3) P3_3;
__sbit __at (0xB0 + 4) P3_4;
__sbit __at (0xB0 + 5) P3_5;
__sbit __at (0xB0 + 6) P3_6;
__sbit __at (0xB0 + 7) P3_7;
#endif /* CH552_H */
