#pragma once

#include "gpio.hpp"

class L298N {
public:
    static void init() {
        // Left engine (IN1, IN2)
        // Front
        pinMode(DDRD, DDD4, PinMode::OUTPUT);
        // Rear
        pinMode(DDRD, DDD5, PinMode::OUTPUT);

        // Right engine
        // Front
        pinMode(DDRD, DDD6, PinMode::OUTPUT);
        // Rear
        pinMode(DDRD, DDD7, PinMode::OUTPUT);
        
        // PWM to adjust engines' speed 
        pinMode(DDRB, DDB3, PinMode::OUTPUT);
        // TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);        
        // Fast PWM, Clear OCR2A on compare match, 64 prescaler
        TCCR2A = 0x83;
        TCCR2B = (1 << CS22);
        
        setSpeed(0);
        stop();
    }

    static void setSpeed(uint8_t speed) {
        // range 0-255
        OCR2A = speed;
    }

    static void stop() {
        digitalWrite(PORTD, PD4, PinState::LOW);
        digitalWrite(PORTD, PD5, PinState::LOW);
        digitalWrite(PORTD, PD6, PinState::LOW);
        digitalWrite(PORTD, PD7, PinState::LOW);
    }

    static void forward() {
        digitalWrite(PORTD, PD4, PinState::HIGH);
        digitalWrite(PORTD, PD5, PinState::LOW);
        digitalWrite(PORTD, PD6, PinState::HIGH);
        digitalWrite(PORTD, PD7, PinState::LOW);
    }

    static void backward() { 
        digitalWrite(PORTD, PD4, PinState::LOW);
        digitalWrite(PORTD, PD5, PinState::HIGH);
        digitalWrite(PORTD, PD6, PinState::LOW);
        digitalWrite(PORTD, PD7, PinState::HIGH);
    }

    static void turnLeft() {
        digitalWrite(PORTD, PD4, PinState::LOW);
        digitalWrite(PORTD, PD5, PinState::HIGH);

        digitalWrite(PORTD, PD6, PinState::HIGH);
        digitalWrite(PORTD, PD7, PinState::LOW);
    }

    static void turnRight() {
        digitalWrite(PORTD, PD4, PinState::HIGH);
        digitalWrite(PORTD, PD5, PinState::LOW);

        digitalWrite(PORTD, PD6, PinState::LOW);
        digitalWrite(PORTD, PD7, PinState::HIGH);
    }
};