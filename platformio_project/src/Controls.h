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

  bool isUpPressed() { return digitalRead(upPin) == LOW; }

  bool isDownPressed() { return digitalRead(downPin) == LOW; }

 private:
  int upPin;
  int downPin;
};

class Buttons {
 public:
  Buttons(int aPin, int bPin, int cPin) : aPin(aPin), bPin(bPin), cPin(cPin) {}

  void begin() {
    pinMode(aPin, INPUT_PULLUP);
    pinMode(bPin, INPUT_PULLUP);
    pinMode(cPin, INPUT_PULLUP);
  }

  bool isAPressed() { return digitalRead(aPin) == LOW; }

  bool isBPressed() { return digitalRead(bPin) == LOW; }

  bool isCPressed() { return digitalRead(cPin) == LOW; }

 private:
  int aPin;
  int bPin;
  int cPin;
};

Buttons buttons(WIO_KEY_A, WIO_KEY_B, WIO_KEY_C);

#endif
