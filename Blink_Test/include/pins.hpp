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

void pwmInit(void) {
    pinMode(DDRD, PD3, PinMode::OUTPUT);

    TCCR2A |= (1 << WGM20) | (1 << WGM21);
    TCCR2A |= (1 << COM2B1);
    TCCR2B |= (1 << CS22);
}

void analogWritePD3(uint8_t duty) {
    OCR2B = duty;
}

class UART {
public:
    static void init(uint32_t baud = 9600) {
        uint16_t ubrr = (F_CPU / (16 * baud)) - 1;
        UBRR0H = (ubrr >> 8);
        UBRR0L = ubrr;
        UCSR0B = (1 << TXEN0); // Włącz nadajnik (TX)
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Format 8n1
    }

    static void sendChar(char c) {
        while (!(UCSR0A & (1 << UDRE0))); // Czekaj na wolny bufor
        UDR0 = c;
    }

    static void sendString(const char* str) {
        while (*str) sendChar(*str++);
    }

    // Pomocnicza funkcja do wypisywania liczb w HEX (np. 0x80)
    static void sendHex8(uint8_t val) {
        const char hexChars[] = "0123456789ABCDEF";
        sendString("0x");
        sendChar(hexChars[(val >> 4) & 0x0F]);
        sendChar(hexChars[val & 0x0F]);
    }
};