#pragma once

#include <avr/io.h>

enum class PinState : bool {
  LOW = false,
  HIGH = true
};

enum class PinMode : bool {
  INPUT = false,
  OUTPUT = true,
  DISABLE_PULLUP = false,
  ENABLE_PULLUP = true
};

// DDRx - Data Direction Register
inline void pinMode(volatile uint8_t &ddr, uint8_t pin, PinMode mode) {
  if (mode == PinMode::OUTPUT) {
    ddr |= (1 << pin);
  } else {
    ddr &= ~(1 << pin);
  }
}

// PORTx - Data Register
inline void digitalWrite(volatile uint8_t &port, uint8_t pin, PinState state) {
  if (state == PinState::HIGH) {
    port |= (1 << pin);
  } else {
    port &= ~(1 << pin);
  }
}

// PINx - Input Pin Adress Register
inline PinState digitalRead(volatile uint8_t &pin_reg, uint8_t pin) {
  return (pin_reg & (1 << pin)) ? PinState::HIGH : PinState::LOW;
}

// AVR microcontroller switches between states when the corresponding bit in the PIN register is set to 1.
inline void digitalToggle(volatile uint8_t &pin_reg, uint8_t pin) {
  pin_reg = (1 << pin);
}