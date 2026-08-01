#include "pins.hpp"
#include <util/delay.h>

// Used PORTD to drive the 7-segment display. The mapping of segments to PORTD pins is as follows:
// Segment A -> PD0
// Segment B -> PD1
// Segment C -> PD2
// Segment D -> PD3
// Segment E -> PD4
// Segment F -> PD5
// Segment G -> PD6

enum class SevenSegmentDigit: uint8_t {
    ZERO = 0x3F, // 0b00111111
    ONE = 0x06,  // 0b00000110
    TWO = 0x5B,  // 0b01011011
    THREE = 0x4F, // 0b01001111
    FOUR = 0x66, // 0b01100110
    FIVE = 0x6D, // 0b01101101
    SIX = 0x7D,  // 0b01111101
    SEVEN = 0x07, // 0b00000111
    EIGHT = 0x7F, // 0b01111111
    NINE = 0x6F // 0b01101111
};

constexpr uint8_t digits[] = {
    static_cast<uint8_t>(SevenSegmentDigit::ZERO),
    static_cast<uint8_t>(SevenSegmentDigit::ONE),
    static_cast<uint8_t>(SevenSegmentDigit::TWO),
    static_cast<uint8_t>(SevenSegmentDigit::THREE),
    static_cast<uint8_t>(SevenSegmentDigit::FOUR),
    static_cast<uint8_t>(SevenSegmentDigit::FIVE),
    static_cast<uint8_t>(SevenSegmentDigit::SIX),
    static_cast<uint8_t>(SevenSegmentDigit::SEVEN),
    static_cast<uint8_t>(SevenSegmentDigit::EIGHT),
    static_cast<uint8_t>(SevenSegmentDigit::NINE)
};

int main(void) { 
    DDRD = 0xFF; // Set all pins of PORTD as output
    PORTD = 0x00; // Initialize PORTD to LOW

    while (true) {
        for (uint8_t i{0}; i <= 9; ++i) {
            PORTD = digits[i];
            _delay_ms(1000); // Wait for 1 second
        }
    }

    return 0;
}