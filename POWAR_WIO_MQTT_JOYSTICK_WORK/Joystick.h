#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>

class Joystick {
public:
  Joystick(int upPin, int downPin) : upPin(upPin), downPin(downPin) {}

  void begin() {
    pinMode(upPin, INPUT_PULLUP);
    pinMode(downPin, INPUT_PULLUP);
  }

  bool isUpPressed() {
    return digitalRead(upPin) == LOW;
  }

  bool isDownPressed() {
    return digitalRead(downPin) == LOW;
  }

private:
  int upPin;
  int downPin;
};

#endif
