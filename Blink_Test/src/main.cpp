#include "gpio.hpp"
#include <util/delay.h>

int main(void) { 
    pinMode(DDRB, DDB5, PinMode::OUTPUT);
    digitalWrite(PORTB, PB5, PinState::LOW);
    unsigned char data{};
    UART::init();

    while (true) {
        data = UART::receive();
        if (data == 'a') {
            digitalWrite(PORTB, PB5, PinState::HIGH);
        }
        if (data == 'b') {
            digitalWrite(PORTB, PB5, PinState::LOW);
        }
    }

    return 0;
}