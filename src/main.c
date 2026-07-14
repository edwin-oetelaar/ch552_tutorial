#include <ch554.h>  // De intel_mcs51 toolchain gebruikt ch554.h ook voor de CH552

// Handmatige delay-functie voor 8051 kloksnelheid
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 1200; j++) {
            __asm__("nop"); // Assembly No-Operation
        }
    }
}

void main(void) {
    // Stel Pin 1.4 in als Push-Pull Output (CH552 register configuratie)
    P1_MOD_OC &= ~(1 << 4);
    P1_DIR_PU |= (1 << 4);

    while(1) {
        P1_4 = 1;  // LED Aan
        delay_ms(500);
        P1_4 = 0;  // LED Uit
        delay_ms(500);
    }
}
