#pragma once

#include "gpio.hpp"

class SG90 {
public:
  static void init() {
    pinMode(DDRB, DDB1, PinMode::OUTPUT);
    digitalWrite(PORTB, PB1, PinState::LOW);

    // // Configure timer to Fast PWM/CTC (Mode 14) (Clear Timer on Compare match with ICR1 as TOP)
    // TCCR1A &= ~(1 << WGM10) & ~(1 << WGM11);
    // TCCR1B |= (1 << WGM12) | (1 << WGM13);
    
    // // Prescaler 8
    // TCCR1B |= (1 << CS11);
    // TCCR1B &= ~(1 << CS10) & ~(1 << CS12);

    // // Output compare mode 
    // TCCR1A &= ~(1 << COM1A1);
    // TCCR1A |= (1 << COM1A0);

    TCCR1A = 0x82;
    TCCR1B = 0x1A;

    // Reset timer to 0
    TCNT1 = 0;

    // Register TOP for 20ms period
    ICR1 = 40000;

    // Start position (in the middle)
    OCR1A = 3000;

    setAngle(0);
    }

  static void setAngle(uint8_t angle) {
    if (angle > 180) angle = 180;
    // range 1ms-2ms
    // OCR1A = 2000 + (angle * 2000) / 180
    // OCR1A = 2000 + ((static_cast<uint16_t>(angle) * 100) / 9);
    
    // range 0.5ms-2.5ms
    // OCR1A = 1000 + (angle * 4000) / 180
    OCR1A = 1000 + ((static_cast<uint16_t>(angle) * 200) / 9);
  }
};