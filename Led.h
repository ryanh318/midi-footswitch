unsigned long ledTimer;

class Led {
  private:
    byte pin;
    bool solid = false;
    bool blink = false;
   void on() {
      digitalWrite(pin, HIGH);
    }
    void off() {
      digitalWrite(pin, LOW);
    }
  public:
    Led(byte pin) {
      this->pin = pin;
      init();
    }
    void init() {
      pinMode(pin, OUTPUT);
      off();
    }
    void display() {
      if(millis() - ledTimer < 100){
        solid!=blink  ? on() : off();
      } else if (millis() - ledTimer < 200) {
        solid ? on() : off();
      } else if (millis() - ledTimer < 300) {
        blink || solid? on() : off() ;
      } else if (millis() - ledTimer < 400) {
        solid ? on() : off();
      } else if (millis() - ledTimer < 600) {
        solid ? on() : off();
      } else if (millis() - ledTimer > 900) {
        solid!=blink  ? on() : off();
        ledTimer = millis();  
      }
    }
    void setBlink(bool blink) {
      this->blink = blink;
    }
    void setSolid(bool solid) {
      this->solid = solid;
    }
};


enum LedMode {
  STATIC,
  STATIC_INVERSE,
  BLINK,
  BLINK_INVERSE,
};
