#include "pins.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect) {
    digitalWrite(PORTB, PB5, PinState::HIGH);
}

ISR(INT1_vect) {
    digitalWrite(PORTB, PB5, PinState::LOW);
}

int main(void) {
    pinMode(DDRB, PB5, PinMode::OUTPUT); // Set PB5 as output
    digitalWrite(PORTB, PB5, PinState::LOW); // Set PB5 low

    pinMode(DDRD, PD2, PinMode::INPUT);  // Set PD2 as input
    pinMode(DDRD, PD3, PinMode::INPUT);  // Set PD3 as input

    // Page 12.2.2
    EIMSK = 0x03; // Enable external interrupts INT0 and INT1 
    // Page 12.2.1
    EICRA = 0x0F; // Set INT0 and INT1 to trigger on rising edge

    // digitalWrite(SREG, SREG_I, PinState::HIGH); // Enable global interrupts (7th bit of SREG)
    // sei(); // Enable global interrupts
    // Page 6.3.1
    SREG = 0x80; // Set the I-bit in SREG to enable interrupts

    while (true) {
        
    }

    return 0;
}