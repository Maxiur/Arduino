#include "gpio.hpp"
#include "servo.hpp"
#include "hcsr04.hpp"
#include "l298n.hpp"
#include <util/delay.h>
#include <avr/interrupt.h>

// ---------------- OPTIMAL FOR 120 SPEED -------------------
// Distance for HCSR04
constexpr uint8_t DISTANCE_RANGE{30};
// Time for rotate 90 degree
constexpr uint16_t TIME_CHASSIS_ROTATE{800};
// Time for rotate Servo by 90 degree
constexpr uint16_t TIME_SERVO_FULL_SCAN{500};
// Time for micro-rotate Servo
constexpr uint16_t TIME_SERVO_MICRO_STEP{125};
// Time for HCSR04 trigger delay to receive data
constexpr uint16_t TIME_HCSR_DELAY{60};

// Measuring distance
template <uint16_t moveTime = TIME_SERVO_FULL_SCAN>
inline uint16_t measureDistance(uint8_t angle) {
    SG90::setAngle(angle);
    _delay_ms(moveTime);
    HCSR04::trigger();
    _delay_ms(TIME_HCSR_DELAY);

    return HCSR04::isReady() ? HCSR04::getDistanceInCm() : 0;
}

// Casting to real distance
template <uint16_t moveTime = TIME_SERVO_FULL_SCAN>
inline uint16_t getClearDistance(uint8_t angle) {
    uint16_t distance = measureDistance<moveTime>(angle);
    // if there is huge distance, HCSR04 returns 0
    return (distance == 0) ? UINT16_MAX : distance;
}

// Is path valid
template <uint16_t moveTime = TIME_SERVO_FULL_SCAN>
inline bool isPathClear(uint8_t angle) {
    return getClearDistance<moveTime>(angle) > DISTANCE_RANGE;
}

inline bool scan() {
    uint16_t leftDistance{getClearDistance(180)};
    uint16_t rightDistance{getClearDistance(0)};
    uint16_t maxDistance{(leftDistance > rightDistance) ? leftDistance : rightDistance};

    if (maxDistance > DISTANCE_RANGE) {
        (leftDistance > rightDistance) ? L298N::turnLeft() : L298N::turnRight();
        SG90::setAngle(90);
        _delay_ms(TIME_CHASSIS_ROTATE);
        L298N::stop();
        return true;
    }

    // Both sides are blocked -> do 180 degree rotate
    L298N::turnLeft();
    SG90::setAngle(90);
    _delay_ms(TIME_CHASSIS_ROTATE * 2);
    L298N::stop();
    return false;
}

inline void checkPath(uint8_t angle) {
    if (!isPathClear<TIME_SERVO_MICRO_STEP>(angle)) {
        L298N::stop();

        while (!scan()) {}
    }
    else {
        L298N::forward();
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
        checkPath(65);
        checkPath(90);
        checkPath(115);
    }
    return 0;
}   