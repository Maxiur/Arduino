#pragma once

#include <avr/io.h>

/**
 * @brief Represents the logical state of a digital pin.
 */
enum class PinState : bool {
  LOW = false,
  HIGH = true
};

/**
 * @brief Represents the direction or pull-up configuration of a digital pin.
 */
enum class PinMode : bool {
  INPUT = false,
  OUTPUT = true
};

/**
 * @brief Configures the specified pin to behave as an input or output.
 * @param ddr Reference to the Data Direction Register (e.g., DDRB, DDRD).
 * @param pin Pin index (0 - 7)
 * @param mode Type PinMode::INPUT or PinMode::OUTPUT
 */
inline void pinMode(volatile uint8_t &ddr, uint8_t pin, PinMode mode) {
  if (mode == PinMode::OUTPUT) {
    ddr |= (1 << pin);
  } else {
    ddr &= ~(1 << pin);
  }
}


/**
 * @brief Writes a HIGH or LOW value to a digital pin.
 * @param port Reference to the Port Data Register (e.g., PORTB, PORTD).
 * @param pin Pin index (0 to 7).
 * @param state Logical state to apply (PinState::HIGH or PinState::LOW).
 */
inline void digitalWrite(volatile uint8_t &port, uint8_t pin, PinState state) {
  if (state == PinState::HIGH) {
    port |= (1 << pin);
  } else {
    port &= ~(1 << pin);
  }
}


/**
 * @brief Reads the value from a specified digital pin.
 * @param pin_reg Reference to the Input Pins Address Register (e.g., PINB, PIND).
 * @param pin Pin index (0 to 7).
 * @return PinState Current logical state of the pin.
 */
inline PinState digitalRead(volatile uint8_t &pin_reg, uint8_t pin) {
  return (pin_reg & (1 << pin)) ? PinState::HIGH : PinState::LOW;
}

/**
 * @brief AVR microcontroller switches between states when the corresponding bit in the PIN register is set to 1.
 * @param pin_reg Reference to the Input Pins Address Register (e.g., PINB, PIND).
 * @param pin Pin index (0 to 7).
 */
inline void digitalToggle(volatile uint8_t &pin_reg, uint8_t pin) {
  pin_reg = (1 << pin);
}

class ADCReader {
public:
  static void init() {
    // Set reference to NO AVCC and input to ADC0
    ADMUX &= ~(1 << REFS1);
    ADMUX |= (1 << REFS0);

    // Set data aligment in data register to right
    ADMUX &= ~(1 << ADLAR);

    // Page 23.9.2
    // Set prescaler to 128
    // Clock 16MHz / 128 = 125,000 Hz
    // Enable ADC and ADC auto trigger enable and ADC interrupt disable
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Page 23.9.1 Table 23-4 
    // Select ADC0 as input channel
    ADMUX &= (~(1 << MUX3)) & (~(1 << MUX2)) & (~(1 << MUX1)) & (~(1 << MUX0));
  }

  [[nodiscard]] static uint16_t analogRead() {
    // Start the convension
    ADCSRA |= (1 << ADSC);

    // wait until flag have fallen
    while (ADCSRA & (1 << ADSC));

    return ADC;
  }
};

/**
 * @brief Driver class for hardware UART (USART0) communication.
 */
class UART {
public:
    /**
     * @brief Initializes the UART interface (8-bit (8n1), double speed U2X0=1, 9600 baud rate @ 16MHz).
     */
    static void init();

    /**
     * @brief Blocks until a byte is received via UART.
     * @return The received character byte.
     */
    [[nodiscard]] static unsigned char receive();

    /**
     * @brief Transmits a single character over UART (blocking).
     * @param ch Character to send.
     */
    static void send(char ch);
};