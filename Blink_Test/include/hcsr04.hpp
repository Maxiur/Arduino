#pragma once

#include "pins.hpp"
#include <avr/interrupt.h>
#include <util/delay.h>

class HCSR04 {
public:
    static void init() {
        // Trigger Pin
        pinMode(DDRD, DDD3, PinMode::OUTPUT);
        // Echo Pin
        pinMode(DDRD, DDD2, PinMode::INPUT);
        
        // ISC01 = 0, ISC00 = 1 (Any logical change)
        EICRA = 0x01;

        // Enable interrupt INT0
        EIMSK |= (1 << INT0);

        sei();
    }

    static void trigger() {
        digitalWrite(PORTD, PD3, PinState::HIGH);
        _delay_us(10);
        digitalWrite(PORTD, PD3, PinState::LOW);
    }

    static uint16_t getDistanceInCm() {
        uint16_t duration;
        cli();
        duration = echo_duration;
        new_data = false;
        sei();
        return duration / 118;
    }

    static bool isReady() {
        return new_data;
    }

    static void handleInterrupt() {
        // Rising Edge, remember start
        if(digitalRead(PIND, PIND2) == PinState::HIGH) {
            echo_start = TCNT1;
        }
        // Falling Edge, Echo returned, calculate duration
        else {
            uint16_t now = TCNT1;
            if (now >= echo_start) {
                echo_duration = now - echo_start;
            }
            else {
                echo_duration = (40000 - echo_start) + now;
            }
            new_data = true;
        }

    }

private:
    static volatile uint16_t echo_start;
    static volatile uint16_t echo_duration;
    static volatile bool new_data;
};

inline volatile uint16_t HCSR04::echo_start = 0;
inline volatile uint16_t HCSR04::echo_duration = 0;
inline volatile bool HCSR04::new_data = false;

ISR(INT0_vect) {
    HCSR04::handleInterrupt();
}