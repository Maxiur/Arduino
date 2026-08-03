#include "pins.hpp"
#include <util/delay.h>
int main(void) {
    // Fast PWM mode, 10-bit resolution
    TCCR1A |= (1 << WGM10) | (1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);

    // Set PWN mode to non-inverting
    TCCR1A |= (1 << COM1A1);
    TCCR1A &= ~(1 << COM1A0);

    // Precaler for the timer for setting PWN frequency (64)
    // Clock 16MHz -> 16,000,000 / 64 = 250,000 Hz
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TCCR1B &= ~(1 << CS12);

    // Set the output pin for PWM (OC1A) as output
    pinMode(DDRB, PB1, PinMode::OUTPUT);
 
    while (true) {
        // Set the duty cycle
        // 0-1023 for 10-bit resolution
        for (uint16_t dutyCycle = 0; dutyCycle <= 1023; ++dutyCycle) {
            OCR1A = dutyCycle;
            _delay_ms(2);
        }
        _delay_ms(1000);

        for (uint16_t dutyCycle = 1023; dutyCycle > 0; --dutyCycle) {
            OCR1A = dutyCycle;
            _delay_ms(2);
        }
        _delay_ms(1000);
        
    }

    return 0;
}