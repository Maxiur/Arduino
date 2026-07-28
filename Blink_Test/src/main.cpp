#include "pins.hpp"
#include <util/delay.h>

int main(void) {
  // set PB5 as output
  pinMode(DDRB, PB5, PinMode::OUTPUT);

  while (true) {
    // toggle the state of PB5 to turn the LED on/off
    digitalToggle(PINB, PB5);
    _delay_ms(200);
  }

  return 0;
}