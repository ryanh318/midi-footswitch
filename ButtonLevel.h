class ButtonLevel {
    ButtonConfig config;
  private:
    byte button;
    byte shift;
    byte level;
    ButtonCommand command;

  public:
    ButtonLevel(byte button, byte shift, byte level, ButtonCommand command, ButtonConfig config) {
      this->button = button;
      this->shift = shift;
      this->level = level;
      this->command = command;
      this->config = config;
    }

    void checkAndActivate() {
      if(config == PRESS_AND_RELEASE) { if(buttonState.validatePressAndRelease(button)) { activate(); }  }
      else if(config == PRESS_AND_HOLD) { if(buttonState.validatePressAndHold(button, false)) { activate(); }  }
      else if(config == DOUBLE_PRESS_AND_HOLD) { if(buttonState.validateDoublePressAndHold(button, false)) { activate(); }  }
      else if(config == SHIFT_PRESS_AND_RELEASE) { if(buttonState.validateShiftPressAndRelease(shift, button)) { activate(); }  }
      else if(config == SHIFT_PRESS_AND_HOLD) { if(buttonState.validateShiftPressAndHold(shift, button, false)) { activate(); }  }
      else if(config == SHIFT_DOUBLE_PRESS_AND_HOLD) { if(buttonState.validateShiftDoublePressAndHold(shift, button, false)) { activate(); }  }
      else if(config == PRESS_AND_HOLD_TOGGLE) { if(buttonState.validatePressAndHold(button, true)) { activate(); }  }
      else if(config == DOUBLE_PRESS_AND_HOLD_TOGGLE) { if(buttonState.validateDoublePressAndHold(button, true)) { activate(); }  }
      else if(config == SHIFT_PRESS_AND_HOLD_TOGGLE) { if(buttonState.validateShiftPressAndHold(shift, button, true)) { activate(); }  }
    }

    void activate() {
      if(command == TOGGLE_MUTE) levels[level].toggleMute();
      if(command == MUTE) levels[level].muteOn();
      if(command == UNMUTE) levels[level].muteOff();
      if(command == INCREMENT_DOUBLE) levels[level].increment();
      if(command == DECREMENT_DOUBLE) levels[level].decrement();
      if(command == INCREMENT || command == INCREMENT_DOUBLE) levels[level].increment();
      if(command == DECREMENT || command == DECREMENT_DOUBLE) levels[level].decrement();
      levels[level].sendMidi();
    }
};
