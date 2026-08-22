#include "gpio.hpp"
#include "servo.hpp"
#include "hcsr04.hpp"
#include "l298n.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

constexpr int8_t DISTANCE_RANGE{30};
constexpr int16_t TimeToRotate{400};
constexpr int16_t TimeToScan{600};

inline bool scan() {
    // Check left side
    SG90::setAngle(180);
    _delay_ms(TimeToScan);
    HCSR04::trigger();
    _delay_ms(60);

    if (HCSR04::isReady()) {
        uint16_t distance = HCSR04::getDistanceInCm();

        if (distance > DISTANCE_RANGE) {
            L298N::turnLeft();
            _delay_ms(TimeToRotate);
            L298N::stop();
            SG90::setAngle(90);
            return true;
        }
    }

    // Check right side
    SG90::setAngle(0);
    _delay_ms(TimeToScan);
    HCSR04::trigger();
    _delay_ms(60);

    if (HCSR04::isReady()) {
        uint16_t distance = HCSR04::getDistanceInCm();

        if (distance > DISTANCE_RANGE) {
            L298N::turnRight();
            _delay_ms(TimeToRotate);
            L298N::stop();
            SG90::setAngle(90);
            return true;
        }
    }

    L298N::turnLeft();
    _delay_ms(TimeToRotate * 2);
    L298N::stop();
    SG90::setAngle(90);
    return false;
}

int main(void) {
    // Turn Off Analog Pins A0-A5
    DIDR0 = 0x3F;
    // Turn Off TWI(I2C), SPI, ADC, Timer0
    PRR = (1 << PRTWI) | (1 << PRSPI) | (1 << PRADC) | (1 << PRTIM0);
    
    L298N::init();
    HCSR04::init();
    SG90::init();

    L298N::setSpeed(120);

    while (true) {
        SG90::setAngle(90);
        _delay_ms(TimeToRotate >> 2);
        HCSR04::trigger();
        _delay_ms(60);

        if (HCSR04::isReady()) {
            uint16_t distance = HCSR04::getDistanceInCm();

            if (distance < DISTANCE_RANGE) {
                L298N::stop();
                if (scan()) {
                    _delay_ms(1000);
                    scan();
                    L298N::forward();
                } else {
                    scan();
                }
            }
        }

        SG90::setAngle(70);
        _delay_ms(TimeToRotate >> 2);
        HCSR04::trigger();
        _delay_ms(60);

        if (HCSR04::isReady()) {
            uint16_t distance = HCSR04::getDistanceInCm();

            if (distance < DISTANCE_RANGE) {
                L298N::stop();
                if (scan()) {
                    _delay_ms(1000);
                    scan();
                    L298N::forward();
                } else {
                    scan();
                }
            }
        }

        SG90::setAngle(90);
        _delay_ms(TimeToRotate >> 2);
        HCSR04::trigger();
        _delay_ms(60);

        if (HCSR04::isReady()) {
            uint16_t distance = HCSR04::getDistanceInCm();

            if (distance < DISTANCE_RANGE) {
                L298N::stop();
                if (scan()) {
                    _delay_ms(1000);
                    scan();
                    L298N::forward();
                } else {
                    scan();
                }
            }
        }

        SG90::setAngle(110);
        _delay_ms(TimeToRotate >> 2);
        HCSR04::trigger();
        _delay_ms(60);

        if (HCSR04::isReady()) {
            uint16_t distance = HCSR04::getDistanceInCm();

            if (distance < DISTANCE_RANGE) {
                L298N::stop();
                if (scan()) {
                    _delay_ms(1000);
                    scan();
                    L298N::forward();
                } else {
                    scan();
                }
            }
        }
    }
    return 0;
}   