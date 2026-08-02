#include "pins.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect) {
    // Reset timer counter
    TCNT1 = 0;

    // Toggle PB5 led
    digitalToggle(PINB, PB5);
}

int main(void) {
    pinMode(DDRB, PB5, PinMode::OUTPUT); // Set PB5 as output pin
    digitalWrite(PINB, PB5, PinState::LOW); // Set PB5 low

    sei(); // Enable global interrupts

    TIMSK1 |= (1<< OCIE1A); // Enable Timer1 output compare interrupt

    // Normal Mode for timer
    TCCR1A &= (~(1 << WGM10)) & (~(1 << WGM11));
    TCCR1B &= (~(1 << WGM12)) & (~(1 << WGM13));
    

    // Output compare mode
    TCCR1A &= ~(1 << COM1A1);
    TCCR1A |= (1 << COM1A0);

    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCCR1B &= ~(1 << CS11);

    // Start timer with initial value
    TCNT1 = 0;

    // Set output compare value for 1 second delay
    OCR1A = 15624;

    // Using PB1 as output pin
    pinMode(DDRB, PB1, PinMode::OUTPUT);

    while (true) {
        
    }

    return 0;
}