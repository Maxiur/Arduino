#include "pins.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(ADC_vect) {
    // Read the ADC value from the ADC data register
    OCR1A = ADC; // Set the PWM duty cycle based on the ADC value
}

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

    //--------------------------------------------

    // Set the output pin for PWM (OC1A) as output
    pinMode(DDRB, PB1, PinMode::OUTPUT);

    sei(); // Enable global interrupts

    // Set reference to NO AVCC and input to ADC0
    ADMUX &= (~(1 << REFS1) & ~(1 << REFS0));

    // Set data aligment in data register to right
    ADMUX &= ~(1 << ADLAR);
    
    // Page 23.9.2
    // Set prescaler to 128
    // Clock 16MHz / 128 = 125,000 Hz
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // Enable ADC and ADC auto trigger enable and ADC interrupt enable
    ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE);

    // Start the convension
    ADCSRA |= (1 << ADSC);
    
    // Page 23.9.1 Table 23-4 
    // Select ADC0 as input channel
    ADMUX &= (~(1 << MUX3)) & (~(1 << MUX2)) & (~(1 << MUX1)) & (~(1 << MUX0));

    while (true) {
        
    }

    return 0;
}