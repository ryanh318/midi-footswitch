#include "HardwareSerial.h"
class Button {
  private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
  public:
    Button(byte pin) {
      this->pin = pin;
      lastReading = LOW;
      init();
    }
    void init() {
      pinMode(pin, INPUT_PULLUP);
      update();
    }
    void update() {
      byte newReading = digitalRead(pin);
      
      if (newReading != lastReading) {
        lastDebounceTime = millis();
      }
      if (millis() - lastDebounceTime > debounceDelay) {
        state = newReading;
      }
      if (newReading != lastReading) {
      }
      lastReading = newReading;
    }
    byte getState() {
      update();
      return state;
    }
    bool isPressed() {
      return (getState() == LOW);
    }
    bool lastIsPressedReading() {
      return (state == LOW);
    }
};

enum ButtonConfig {
  PRESS_AND_RELEASE,
  PRESS_AND_HOLD,
  PRESS_AND_HOLD_TOGGLE,
  DOUBLE_PRESS_AND_RELEASE,
  DOUBLE_PRESS_AND_HOLD,
  DOUBLE_PRESS_AND_HOLD_TOGGLE,
  SHIFT_PRESS_AND_RELEASE,
  SHIFT_PRESS_AND_HOLD,
  SHIFT_PRESS_AND_HOLD_TOGGLE,
  SHIFT_DOUBLE_PRESS_AND_HOLD,
};

enum ButtonCommand {
  MUTE,
  UNMUTE,
  TOGGLE_MUTE,
  INCREMENT,
  INCREMENT_DOUBLE,
  DECREMENT,
  DECREMENT_DOUBLE,
};
