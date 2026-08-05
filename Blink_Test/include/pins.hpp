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

class UART {
public:
    static void init() {
        // Set RXENn and TXENn bits in UCSRnB to enable receiver and transmitter (4, 3 bit)
        // Set UCSZn2 bit to 0 (8-bit communication) (2 bit)
        UCSR0B = 0x18;

        // Set datasize for communication Asynchronous USART
        // Disable parity communication
        // Set stop bit to 1 bit (one time)
        // Set UCSZn0 and UCSZn1 bits to 1 (8-bit communication) (1, 2 bit)
        UCSR0C = 0x06;

        // Set speed of communication - double the USART Transmission speed (1 bit)
        UCSR0A = 0x02;

        // Set baud rate to 9600 (UBRN0 = 207 for U2X0 = 1)
        UBRR0 = 0xCF;
    }

    [[nodiscard]] static unsigned char receive() {
        // Wait for data to be received
        // if flag RXC0 if rising
        // then hardware will clean the flag
        while (!(UCSR0A & (1 << RXC0)));

        // Get and return received data from buffer
        return UDR0; 
    }

    static void send(char ch) {
        // Wait for register flag to be empty
        while (!(UCSR0A & (1 << UDRE0)));

        // Send the char
        UDR0 = ch;
    }

};
