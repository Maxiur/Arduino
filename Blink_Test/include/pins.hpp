#pragma once

#include <avr/io.h>

enum class PinState : bool {
  LOW = false,
  HIGH = true
};

enum class PinMode : bool {
  INPUT = false,
  OUTPUT = true
};

inline void pinMode(volatile uint8_t &ddr, uint8_t pin, PinMode mode) {
  if (mode == PinMode::OUTPUT) {
    ddr |= (1 << pin);
  } else {
    ddr &= ~(1 << pin);
  }
}

inline void digitalWrite(volatile uint8_t &PORT, uint8_t PIN, PinState state) {
  if (state == PinState::HIGH) {
    PORT |= (1 << PIN);
  } else {
    PORT &= ~(1 << PIN);
  }
}

inline PinState digitalRead(volatile uint8_t &pin_reg, uint8_t pin) {
  return (pin_reg & (1 << pin)) ? PinState::HIGH : PinState::LOW;
}

// AVR microcontroller switches between states when the corresponding bit in the PIN register is set to 1.
inline void digitalToggle(volatile uint8_t &pin_reg, uint8_t pin) {
  pin_reg = (1 << pin);
}