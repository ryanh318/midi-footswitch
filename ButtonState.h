#include "HardwareSerial.h"
class ButtonState {
  private:
    bool state1[BUTTON_NUM];
    bool state2[BUTTON_NUM];
    bool state3[BUTTON_NUM];
    bool state4[BUTTON_NUM];
    unsigned long time0;
    unsigned long time1;
    unsigned long time2;
    unsigned long time3;
    static const int DOUBLE_PRESS_TIME = 500;
    static const int PRESS_AND_HOLD_TIME = 250;
    static const int PRESS_AND_HOLD_TOGGLE_TIME = 1000;
    static const int DOUBLE_PRESS_AND_HOLD_TIME = 100;

    void init(){
      for(int i=0;i<BUTTON_NUM;i++) {
        state4[i] = false;
        state3[i] = false;
        state2[i] = false;
        state1[i] = false;
      }
    }
    
    bool checkPressAndHoldToggle(int toggleTime) {
        // Check for previous activation and that press has been pressed for long enough 
      if(time0 != time1 && millis() - time1 > toggleTime) {
        // Mark as previously activated
        time0 = time1;
        return true;
      }
      return false;
    }

    bool checkPressAndHold(int pressAndHoldTime) {
      // Check that enough time has elapsed
      if(millis() - time0 > pressAndHoldTime) {
        time0 = millis();
        return true;
      }
      return false;
    }

  public:
    ButtonState() {
      init();
    }

    void update() {
      if(!hasStateChanged()) return;
      for(int i=0;i<BUTTON_NUM;i++) {
        state4[i] = state3[i];
        state3[i] = state2[i];
        state2[i] = state1[i];
        state1[i] = buttons[i].isPressed();
      }
      Serial.print("State1:");
      Serial.print(state1[0]);
      Serial.print(state1[1]);
      Serial.print(state1[2]);
      Serial.print(state1[3]);
      Serial.print("    State2:");
      Serial.print(state2[0]);
      Serial.print(state2[1]);
      Serial.print(state2[2]);
      Serial.println(state2[3]);

      time3 = time2; 
      time2 = time1; 
      time1 = millis();
    }

    unsigned long getLastTime() {
      return time1;
    }

    bool isClear() {
      for(int i=0;i<BUTTON_NUM;i++) {
        if(state1[i] == true) return false;
      }
      return true;
    }

    void clear() {
      for(int i=0;i<BUTTON_NUM;i++) {
        state1[i] = false;
        state2[i] = false;
        state3[i] = false;
        state4[i] = false;
      }
    }
  
    bool hasStateChanged() {
      for(int i=0;i<BUTTON_NUM;i++) {
        if(state1[i] != buttons[i].isPressed()) {
          return true;
        } 
      }
      return false;
    }

    bool validatePressAndRelease(int press) {
      // Check that a press and hold was not just activated
      if(time0 == time2) return false;
      
      //Check that press was pressed then released
      if((!state1[press] && state2[press]) == false) {
        return false;
      }
      // Check that all other buttons were not pressed
      for(int i=0;i<BUTTON_NUM;i++) {
        if(i == press) {
          continue;
        }
        if(state1[i] || state2[i] || state3[i]) {
          return false;
        }
      }
      if(time0 < time1) {
        time0 = time1;
        Serial.println("validatePressAndRelease");

        return true;
      }
      return false;
    }


    bool validateShiftPressAndRelease(int shift, int press) {
      // Check that a press and hold was not just activated
      if(time0 == time2) return false;

      // Check that shift was pressed the whole time
      if((state1[shift] && state2[shift] && state3[shift]) == false) {
        return false;
      }

      // Check that press was pressed in middle state and unpressed in the other two
      if(((!state1[press]) && state2[press] && (!state3[press])) == false) {
        return false;
      }

      // Check that all other buttons were left unpressed
      for(int i=0;i<BUTTON_NUM;i++) {
        if(i == shift || i == press) {
          continue;
        }
        if(state1[i] || state2[i] || state3[i]) {
          return false;
        }
      }

      if(time0 < time1) {
        time0 = time1;
        Serial.println("validateShiftPressAndRelease");

        return true;
      }
      return false;
    }

    bool validatePressAndHold(int press, bool toggle) {
      //Check that press was pressed then released
      if((state1[press] && !state2[press]) == false) {
        return false;
      }
      
      // Check that all other buttons were not pressed
      for(int i=0;i<BUTTON_NUM;i++) {
        if(i == press) {
          continue;
        }
        if(state1[i] || state2[i]) {
          return false;
        }
      }

      if(toggle) {
        if(checkPressAndHoldToggle(PRESS_AND_HOLD_TOGGLE_TIME)) {
          Serial.println("validatePressAndHoldToggle");
          return true;
        }
      } else {
        if(checkPressAndHold(PRESS_AND_HOLD_TIME)) {
          Serial.println("validatePressAndHold");
          return true;
        }
      }
      return false;
    }

    bool validateDoublePressAndHold(int press, bool toggle) {
      //Check that press was pressed then released
      if((state1[press] && !state2[press] && state3[press] && !state4[press]) == false) {
        return false;
      }
      // Check that the double press was quick enough
      if(time1 - time2 > DOUBLE_PRESS_TIME || time2 - time3 > DOUBLE_PRESS_TIME ) return false;

      // Check that all other buttons were not pressed
      for(int i=0;i<BUTTON_NUM;i++) {
        if(i == press) {
          continue;
        }
        if(state1[i] || state2[i] || state3[i] || state4[i]) {
          return false;
        }
      }

      if(toggle) {
        if(checkPressAndHoldToggle(PRESS_AND_HOLD_TOGGLE_TIME)) {
          Serial.println("validateDoublePressAndHoldToggle");
          return true;
        }
      } else {
        if(checkPressAndHold(DOUBLE_PRESS_AND_HOLD_TIME)) {
          Serial.println("validateDoublePressAndHold");
          return true;
        }
      }
      return false;
    }

    bool validateShiftPressAndHold(int shift, int press, bool toggle) {
      // Check that shift was pressed the whole time
      if((state1[shift] && state2[shift]) == false) {
        return false;
      }
      //Check that press was pressed then released
      if((state1[press] && !state2[press]) == false) {
        return false;
      }

      // Check that all other buttons were not pressed
      for(int i=0;i<BUTTON_NUM;i++) {
        if(i == press || i == shift) {
          continue;
        }
        if(state1[i] || state2[i]) {
          return false;
        }
      }

      if(toggle) {
        if(checkPressAndHoldToggle(PRESS_AND_HOLD_TOGGLE_TIME)) {
          Serial.println("validateShiftPressAndHoldToggle");
          return true;
        }
      } else {
        if(checkPressAndHold(PRESS_AND_HOLD_TIME)) {
          Serial.println("validateShiftPressAndHold");
          return true;
        }
      }
      return false;
    }


    bool validateShiftDoublePressAndHold(int shift, int press, bool toggle) {
      // Check that shift was pressed the whole time
      if((state1[shift] && state2[shift] && state3[shift] && state4[shift]) == false) {
        return false;
      }
      //Check that press was pressed then released
      if((state1[press] && !state2[press] && state3[press] && !state4[press]) == false) {
        return false;
      }
      // Check that the double press was quick enough
      if(time1 - time2 > DOUBLE_PRESS_TIME || time2 - time3 > DOUBLE_PRESS_TIME ) return false;

      // Check that all other buttons were not pressed
      for(int i=0;i<BUTTON_NUM;i++) {
        if(i == press || i == shift) {
          continue;
        }
        if(state1[i] || state2[i] || state3[i] || state4[i]) {
          return false;
        }
      }
      if(toggle) {
        if(checkPressAndHoldToggle(PRESS_AND_HOLD_TOGGLE_TIME)) {
          Serial.println("validateShiftDoublePressAndHoldToggle");
          return true;
        }
      } else {
        if(checkPressAndHold(DOUBLE_PRESS_AND_HOLD_TIME)) {
          Serial.println("validateShiftDoublePressAndHold");
          return true;
        }
      }
      return false;
    }
};
