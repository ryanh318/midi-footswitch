
class LedLevel {
  byte led;
  byte level;
  LedMode ledMode;
  public:
    LedLevel(byte led, byte level, LedMode ledMode) {
      this->led = led;
      this->level = level;
      this->ledMode = ledMode;
    }
    void setLedState() {
      if(ledMode == STATIC) leds[led].setSolid(!levels[level].getMute());
      if(ledMode == STATIC_INVERSE) leds[led].setSolid(levels[level].getMute());
      if(ledMode == BLINK) leds[led].setBlink(!levels[level].getMute());
      if(ledMode == BLINK_INVERSE) leds[led].setBlink(levels[level].getMute());
    }
};
