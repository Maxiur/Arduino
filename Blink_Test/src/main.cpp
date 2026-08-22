#include "gpio.hpp"
#include "servo.hpp"
#include "hcsr04.hpp"
#include "l298n.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

// Distance for HCSR04
constexpr uint8_t DISTANCE_RANGE{30};
// Time for rotate 90 degree
constexpr uint16_t TIME_CHASSIS_ROTATE{800};
// Time for rotate Servo by 90 degree
constexpr uint16_t TIME_SERVO_FULL_SCAN{500};
// Time for micro-rotate Servo
constexpr uint16_t TIME_SERVO_MICRO_STEP{125};


inline bool scan() {
    // Check left side
    SG90::setAngle(180);
    _delay_ms(TIME_SERVO_FULL_SCAN);
    HCSR04::trigger();
    _delay_ms(60);

    if (HCSR04::isReady()) {
        uint16_t distance = HCSR04::getDistanceInCm();

        if (distance > DISTANCE_RANGE || distance == 0) {
            L298N::turnLeft();
            _delay_ms(TIME_CHASSIS_ROTATE);
            L298N::stop();
            SG90::setAngle(90);
            return true;
        }
    }

    // Check right side
    SG90::setAngle(0);
    _delay_ms(TIME_SERVO_FULL_SCAN);
    HCSR04::trigger();
    _delay_ms(60);

    if (HCSR04::isReady()) {
        uint16_t distance = HCSR04::getDistanceInCm();

        if (distance > DISTANCE_RANGE || distance == 0) {
            L298N::turnRight();
            _delay_ms(TIME_CHASSIS_ROTATE);
            L298N::stop();
            SG90::setAngle(90);
            return true;
        }
    }

    L298N::turnLeft();
    _delay_ms(TIME_CHASSIS_ROTATE * 2);
    L298N::stop();
    SG90::setAngle(90);
    return false;
}

inline void checkPath(uint8_t angle) {
    SG90::setAngle(angle);
    _delay_ms(TIME_SERVO_MICRO_STEP);
    HCSR04::trigger();
    _delay_ms(60);

    if (HCSR04::isReady()) {
        uint16_t distance = HCSR04::getDistanceInCm();

        if (distance > 0 && distance < DISTANCE_RANGE) {
            L298N::stop();

            while (!scan()) {}

            SG90::setAngle(90);
            _delay_ms(TIME_SERVO_FULL_SCAN);
        }
        else {
            L298N::forward();
        }
    }
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
        checkPath(90);
        checkPath(70);
        checkPath(90);
        checkPath(110);
    }
    return 0;
}   