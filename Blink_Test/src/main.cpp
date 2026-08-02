#include "pins.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(TIMER1_OVF_vect) {
    // Reset the timer count for the next overflow
    TCNT1 = 49911; // 65536 - (16,000,000 / 1024) = 49911

    // Toggle the LED connected to PB5
    digitalToggle(PINB, PB5);
}

int main(void) {
    pinMode(DDRB, PB5, PinMode::OUTPUT); // Set PB5 as output (built-in LED on Arduino Uno
    digitalWrite(PORTB, PB5, PinState::LOW); // Ensure the LED is initially off

    // Page 15.11.1
    // Set Timer to Normal Mode
    TCCR1A &= ~(WGM10 | WGM11); // Clear WGM10 and WGM11 bits for Normal mode
    TCCR1B &= ~(WGM12 | WGM13); // Clear WGM12 and WGM13 bits for Normal mode

    // Load the period for the timer for 1 second delay
    TCNT1 = 49911; // 65536 - (16,000,000 / 1024) = 49911

    // Enable Timer1 overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    // Page 15.11.2
    // Set the Prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
    TCCR1B &= ~(1 << CS11); // Clear CS11
    sei(); // Enable global interrupts

    while (true) {
        
    }

    return 0;
}