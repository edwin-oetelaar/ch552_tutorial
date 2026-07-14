#ifndef CH552_PERIPHERALS_H
#define CH552_PERIPHERALS_H

/* =========================================================================
 * CH552 Peripherals Special Function Registers (SFRs)
 * 
 * Dit bestand bevat de definities van alle unieke CH552 ingebouwde hardware
 * randapparaten zoals de USB Controller, DMA-endpoints, SPI0, ADC, Touch en PWM.
 * 
 * Dit is een waardevolle uitbreiding voor computerarchitectuur lessen om te 
 * laten zien hoe hardware-peripherals door de CPU worden bestuurd via SFRs.
 * ========================================================================= */

// =========================================================================
// 1. SYSTEM CONFIGURATION & CLOCK
// =========================================================================
__sfr __at (0xB1) GLOBAL_CFG;   /* Global Configuration (Boot/Reset flags, LDO disable) */
__sfr __at (0xA1) CHIP_ID;      /* Read-Only Chip ID (0x52 voor CH552) */
__sfr __at (0xA2) XBUS_AUX;     /* Aux-instellingen (Selectie DPTR0/DPTR1, auto-increment) */
__sfr __at (0xA9) WAKE_CTRL;    /* Slaap- en Wake-up interrupts */
__sfr __at (0xFE) RESET_KEEP;   /* Reset status waarde (blijft behouden na reset) */
__sfr __at (0xFF) WDOG_COUNT;   /* Watchdog Timer counter */

// =========================================================================
// 2. POORT CONFIGURATIE & INTERRUPTS
// =========================================================================
__sfr __at (0xC6) PIN_FUNC;     /* Pin-mapping / multiplexing (o.a. UART pins verplaatsen, SPI enable) */
__sfr __at (0xC7) GPIO_IE;      /* Externe GPIO Interrupt en Edge-trigger selectie */

// =========================================================================
// 3. UART1 (Tweede Seriële Hardware Poort)
// =========================================================================
__sfr __at (0xC0) SCON1;        /* UART1 Control Register (Modus, ontvangen toestaan, flags) */
__sfr __at (0xC1) SBUF1;        /* UART1 Data buffer (Zenden/Ontvangen) */
__sfr __at (0xC2) SBAUD1;       /* UART1 Baudrate deler */

// =========================================================================
// 4. SPI0 INTERFACE
// =========================================================================
__sfr __at (0xF8) SPI0_STAT;    /* SPI0 Status register (Transfer completed, FIFO flags) */
__sfr __at (0xF9) SPI0_DATA;    /* SPI0 Data register (Zenden/Ontvangen FIFO) */
__sfr __at (0xFA) SPI0_CTRL;    /* SPI0 Control register (Master/Slave selectie, clock polariteit) */
__sfr __at (0xFB) SPI0_CK_SE;   /* SPI0 Clock deler (in Master mode) */
__sfr __at (0xFB) SPI0_S_PRE;   /* SPI0 Preset data (in Slave mode) */
__sfr __at (0xFC) SPI0_SETUP;   /* SPI0 Setup register (Interrupt enables, bit volgorde MSB/LSB) */

// =========================================================================
// 5. PWM1 & PWM2 (Hardware Pulse-Width Modulation)
// =========================================================================
__sfr __at (0x9B) PWM_DATA2;    /* PWM2 Duty-cycle register */
__sfr __at (0x9C) PWM_DATA1;    /* PWM1 Duty-cycle register */
__sfr __at (0x9D) PWM_CTRL;     /* PWM Control (Output enable, polariteit en interrupt) */
__sfr __at (0x9E) PWM_CK_SE;    /* PWM Clock frequentiedeler */

// =========================================================================
// 6. ADC & ANALOGE COMPARATOR
// =========================================================================
__sfr __at (0x80) ADC_CTRL;     /* ADC Control (Bit-adresseerbaar, o.a. ADC start en kanaalselectie) */
__sfr __at (0x9A) ADC_CFG;      /* ADC Configuratie (Power control, kloksnelheid) */
__sfr __at (0x9F) ADC_DATA;     /* ADC Sampling resultaat byte */

// =========================================================================
// 7. TOUCH-KEY (Capacitieve Touch Meting)
// =========================================================================
__sfr __at (0xC3) TKEY_CTRL;    /* Touch control register (Kanaalkeuze en start meting) */
__sfr __at (0xC4) TKEY_DATL;    /* Touch Data laag byte */
__sfr __at (0xC5) TKEY_DATH;    /* Touch Data hoog byte (bevat tevens veranderings-vlag) */

// =========================================================================
// 8. FLASH-ROM / EEPROM CONTROLLER (DataFlash)
// =========================================================================
__sfr __at (0x84) ROM_ADDR_L;   /* Flash Geheugenadres laag byte */
__sfr __at (0x85) ROM_ADDR_H;   /* Flash Geheugenadres hoog byte */
__sfr __at (0x86) ROM_CTRL;     /* Flash commando register (Write-Only: o.a. schrijven/wissen) */
__sfr __at (0x86) ROM_STATUS;   /* Flash status (Read-Only: operation OK, commando error) */
__sfr __at (0x8E) ROM_DATA_L;   /* Flash data register laag byte (lezen/schrijven) */
__sfr __at (0x8F) ROM_DATA_H;   /* Flash data register hoog byte */

// =========================================================================
// 9. USB DEVICE CONTROLLER (Global Registers)
// =========================================================================
__sfr __at (0x91) USB_C_CTRL;   /* USB Type-C configuratie en pull-up/pull-down weerstanden */
__sfr __at (0xD1) UDEV_CTRL;    /* USB Fysieke poort besturing (o.a. low-speed/full-speed, enable) */
__sfr __at (0xD8) USB_INT_FG;   /* USB Interrupt Vlaggen (Bit-adresseerbaar, o.a. transfer complete) */
__sfr __at (0xD9) USB_INT_ST;   /* USB Interrupt Status (Geeft actieve Endpoint en Token type) */
__sfr __at (0xDA) USB_MIS_ST;   /* USB Diverse statusvlaggen (o.a. SOF ontvangen, bus reset status) */
__sfr __at (0xDB) USB_RX_LEN;   /* USB Ontvangen pakketlengte in bytes */
__sfr __at (0xE1) USB_INT_EN;   /* USB Interrupt Enables (o.a. transfer complete, reset interrupt) */
__sfr __at (0xE2) USB_CTRL;     /* USB Device Control (o.a. DMA en interrupts inschakelen) */
__sfr __at (0xE3) USB_DEV_AD;   /* USB Apparaatadres (toegewezen door host tijdens enumeratie) */

// =========================================================================
// 10. USB ENDPOINTS CONFIGURATIE & DMA ADRESSEN
// =========================================================================
/* Endpoint Control Registers */
__sfr __at (0xDC) UEP0_CTRL;    /* Endpoint 0 Control (Standaard besturings-endpoint) */
__sfr __at (0xD2) UEP1_CTRL;    /* Endpoint 1 Control */
__sfr __at (0xD4) UEP2_CTRL;    /* Endpoint 2 Control */
__sfr __at (0xD6) UEP3_CTRL;    /* Endpoint 3 Control */
__sfr __at (0xDE) UEP4_CTRL;    /* Endpoint 4 Control */

/* Endpoint Transmit Length Registers (Aantal bytes klaar om te zenden per IN transactie) */
__sfr __at (0xDD) UEP0_T_LEN;   /* Endpoint 0 IN Transmit lengte */
__sfr __at (0xD3) UEP1_T_LEN;   /* Endpoint 1 IN Transmit lengte */
__sfr __at (0xD5) UEP2_T_LEN;   /* Endpoint 2 IN Transmit lengte */
__sfr __at (0xD7) UEP3_T_LEN;   /* Endpoint 3 IN Transmit lengte */
__sfr __at (0xDF) UEP4_T_LEN;   /* Endpoint 4 IN Transmit lengte */

/* Endpoint Modus Registers (Configuratie van buffers: Single/Double buffer, IN/OUT/Interrupt) */
__sfr __at (0xE8) UEP4_1_MOD;   /* Mode register voor Endpoint 4 en 1 */
__sfr __at (0xEB) UEP2_3_MOD;   /* Mode register voor Endpoint 2 en 3 */

/* 
 * Endpoint DMA Startadressen (16-bit register gesplitst in Low/High byte)
 * ARCHITECTUUR LES:
 * De USB-hardware schrijft ontvangen data direct weg in het xRAM (DMA), 
 * zonder tussenkomst van de CPU. De CPU stelt hieronder in waar de buffer start.
 * 
 * N.B. Endpoint 0 en Endpoint 4 delen hetzelfde DMA-basisadresregister!
 */
__sfr __at (0xEC) UEP0_DMA_L;   /* Endpoint 0 & 4 DMA Buffer Start Low Byte */
__sfr __at (0xED) UEP0_DMA_H;   /* Endpoint 0 & 4 DMA Buffer Start High Byte */
__sfr __at (0xEE) UEP1_DMA_L;   /* Endpoint 1 DMA Buffer Start Low Byte */
__sfr __at (0xEF) UEP1_DMA_H;   /* Endpoint 1 DMA Buffer Start High Byte */
__sfr __at (0xE4) UEP2_DMA_L;   /* Endpoint 2 DMA Buffer Start Low Byte */
__sfr __at (0xE5) UEP2_DMA_H;   /* Endpoint 2 DMA Buffer Start High Byte */
__sfr __at (0xE6) UEP3_DMA_L;   /* Endpoint 3 DMA Buffer Start Low Byte */
__sfr __at (0xE7) UEP3_DMA_H;   /* Endpoint 3 DMA Buffer Start High Byte */

#endif /* CH552_PERIPHERALS_H */
