#include "gpio.hpp"
#include "servo.hpp"
#include "hcsr04.hpp"
#include "l298n.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

constexpr int8_t DISTANCE_RANGE{15};

inline void scan() {
    // Check left side
    SG90::setAngle(0);
    _delay_ms(200);
    HCSR04::trigger();
    _delay_ms(60);

    if (HCSR04::isReady()) {
        uint16_t distance = HCSR04::getDistanceInCm();

        if (distance > DISTANCE_RANGE) {
            L298N::turnLeft();
            return;
        }
    }

    // Check right side
    SG90::setAngle(180);
    _delay_ms(200);
    HCSR04::trigger();
    _delay_ms(60);

    if (HCSR04::isReady()) {
        uint16_t distance = HCSR04::getDistanceInCm();

        if (distance > DISTANCE_RANGE) {
            L298N::turnRight();
            return;
        }
    }
}

int main(void) {
    L298N::init();
    HCSR04::init();
    SG90::init();

    L298N::setSpeed(150);
    L298N::forward();

    while (true) {
        HCSR04::trigger();
        _delay_ms(60);
        
        if (HCSR04::isReady()) {
            uint16_t distance = HCSR04::getDistanceInCm();

            if (distance > 0 && distance < 15) {
                L298N::stop();
                scan();
                _delay_ms(500);
                L298N::forward();
            }
        }

        
    }
    return 0;
}   