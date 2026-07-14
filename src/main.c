#include "ch552.h"

#define LED_PIN  P3_0

void delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++) {
            __asm__("nop");
        }
    }
}

void main() {
    P3_MOD_OC &= ~(1 << 0);
    P3_DIR_PU |= (1 << 0);

    while (1) {
        LED_PIN = 1;
        delay_ms(500);
        LED_PIN = 0;
        delay_ms(500);
    }
}
