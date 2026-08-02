#include "pins.hpp"
#include <util/delay.h>

void lcd_data(unsigned char data) {
    PORTD = data; // Send data to PORTD
    digitalWrite(PORTB, PB0, PinState::HIGH); // RS = 1 for data
    digitalWrite(PORTB, PB1, PinState::LOW); // RW = 0 for write
    digitalWrite(PORTB, PB2, PinState::HIGH); // Enable clock = 1
    _delay_ms(10); // Wait for data to be latched
    digitalWrite(PORTB, PB2, PinState::LOW); // Enable clock = 0
}

void lcd_command(unsigned char command) {
    PORTD = command;
    digitalWrite(PORTB, PB0, PinState::LOW); // RS = 0 for command
    digitalWrite(PORTB, PB1, PinState::LOW); // RW = 0 for write
    digitalWrite(PORTB, PB2, PinState::HIGH); // Enable clock = 1
    _delay_ms(10); // Wait for command to be latched
    digitalWrite(PORTB, PB2, PinState::LOW); // Enable clock = 0
}

void lcd_string(const unsigned char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

void lcd_init(void) {
    _delay_ms(50); // Wait for LCD to power up

    lcd_command(0x30);
    _delay_ms(5);
    lcd_command(0x30);
    _delay_us(150);
    lcd_command(0x30);

    lcd_command(0x38); // 16 column, 2 row format, 5x7 dots
    lcd_command(0x06); // increment cursor after char is displayed
    lcd_command(0x0C); // Display on, cursor off
    lcd_command(0x01); // Clear display
}

int main(void) { 
    DDRD = 0xFF; // Set all pins of PORTD as output
    DDRB = 0x07; // Set PB0, PB1, PB2 as output
    PORTD = 0x00; // Initialize PORTD to LOW
    PORTB = 0x00; // Initialize PORTB to LOW

    lcd_init(); // Initialize the LCD
    lcd_command(0x80); // Set cursor to the beginning of the first line
    lcd_string("Hello, World!"); // Display a string on the LCD
    _delay_ms(50); // Wait for 50ms
    lcd_command(0xC0); // Set cursor to the beginning of the second line
    lcd_string("LCD Test"); // Display another string on the LCD

    while (true) {
        
    }

    return 0;
}