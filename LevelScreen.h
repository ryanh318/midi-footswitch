static unsigned long lastScreenDraw = -100;
static int lastScreenLevel;

class LevelScreen {
  private:
    byte level;
  public:
    LevelScreen(byte level) {
      this->level = level;
    }

    byte getLevel() {
      return level;
    }
    void swap() {
      
    }
    void cycle() {
      
    }
    static void draw() {

    }

};
