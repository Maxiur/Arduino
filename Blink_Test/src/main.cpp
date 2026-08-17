#include "pins.hpp"
#include "servo.hpp"
#include "hcsr04.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

void printNumber(uint16_t num) {
    if (num == 0) {
        UART::send('0');
        return;
    }
    
    char buf[6];
    int i = 0;
    while (num > 0) {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    // Wyślij cyfry w dobrej kolejności
    while (--i >= 0) {
        UART::send(buf[i]);
    }
}

int main(void) {
    // pinMode(DDRB, DDB5, PinMode::OUTPUT);
    // digitalWrite(PORTB, PB5, PinState::HIGH);
    // SG90::init();
    // HCSR04::init();
    // SG90::setAngle(90);

    // while (true) {
    //     HCSR04::trigger();
    //     _delay_ms(60);

    //     if (HCSR04::isReady()) {
    //         uint16_t dist = HCSR04::getDistanceInCm();

    //         // Jeśli przeszkoda bliżej niż 15 cm - obróć serwo
    //         if (dist > 0 && dist < 15) {
    //             SG90::setAngle(180);
    //         } else {
    //             SG90::setAngle(0);
    //         }
    //     }
    // }

    UART::init();
    ADCReader::init();

    while (true) {
        uint16_t rawValue = ADCReader::analogRead();

        uint8_t percentage = (static_cast<uint16_t>(rawValue) * 25) >> 8;

        UART::send('P');
        UART::send(':');
        printNumber(percentage);
        UART::send('%');

        UART::send('\r');
        UART::send('\n');

        _delay_ms(15000);
    }

    return 0;
}   