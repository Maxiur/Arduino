#include "pins.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

class IRDecoder {
public:
    volatile bool dataReady{false};
    volatile uint8_t address{0};
    volatile uint8_t command{0};

    void init() {
        pinMode(DDRD, PD2, PinMode::INPUT);
        digitalWrite(PORTD, PD2, PinState::HIGH); // Enable pull-up resistor
        
        EICRA |= (1 << ISC01); // Trigger on falling edge
        EICRA &= ~(1 << ISC00);
        EIMSK |= (1 << INT0); // Enable external interrupt INT0

        TCCR1A = 0; // Clear Timer1 control register A
        TCCR1B = (1 << CS11); // Set Timer1 prescaler to 8
    }

    void handleInterrupt() {
        uint16_t duration{TCNT1}; // Read Timer1 value
        TCNT1 = 0; // Reset Timer1

        uint16_t timeUs{duration / 2}; // Convert to microseconds (assuming 16MHz clock and prescaler of 8)
        
        // Identify the header ~13.5 ms
        if (timeUs > 12000 && timeUs < 15000) {
            bitCount = 0;
            rawShiftRegister = 0;
            return;
        }
        
        // Reading bits
        if (bitCount < 32) {
            // Logic '1' is represented by a pulse about 2.25 ms
            if (timeUs > 1800 && timeUs < 2600) {
                rawShiftRegister |= (static_cast<uint32_t>(1) << (31 - bitCount));
            }
            // Logic '0' is represented by a pulse about 1.125 ms, so we ignore

            ++bitCount;

            if (bitCount == 32) {
                address = static_cast<uint8_t>((rawShiftRegister >> 24) & 0xFF);
                command = static_cast<uint8_t>((rawShiftRegister >> 8) & 0xFF);
                dataReady = true;
            }
        }

    }

private:
    volatile uint8_t bitCount{0};
    volatile uint32_t rawShiftRegister{0};
};

IRDecoder irDecoder;

ISR(INT0_vect) {
    irDecoder.handleInterrupt();
}

int main(void) { 
    irDecoder.init();
    UART::init(9600); // Initialize UART with 9600 baud rate
    sei(); // Enable global interrupts

    UART::sendString("IR Decoder Initialized\r\n");

    while (true) {
        if (irDecoder.dataReady) {
            // Process the received address and command
            uint8_t address{irDecoder.address};
            uint8_t command{irDecoder.command};

            // Reset data ready flag
            irDecoder.dataReady = false;
            
            // Other stuff
            UART::sendString("Adres: ");
            UART::sendHex8(address);
            UART::sendString(" | Komenda: ");
            UART::sendHex8(command);
            UART::sendString("\r\n");
        }
    }

    return 0;
}