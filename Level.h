unsigned long int lastNagTime;

class Level {
  private:
    int value=127;
    bool mute=false;
    byte ccChannel;
    bool nag = true;
    byte step = 2;
  public:
    byte getMidiValue() {
      return mute ? 0 : value;
    }

    Level(byte ccChannel) {
      this->ccChannel = ccChannel;
      // this->name = name;
    }

    Level(byte ccChannel, bool mute) {
      this->ccChannel = ccChannel;
      this->mute = mute;
      // this->name = name;
    }

    Level(byte ccChannel, byte value, bool mute) {
      this->value = value;
      this->ccChannel = ccChannel;
      this->mute = mute;
      // this->name = name;
    }

    Level(byte ccChannel, byte value, bool mute, bool nag) {
      this->value = value;
      this->ccChannel = ccChannel;
      this->mute = mute;
      // this->name = name;
      this->nag = nag;
    } 

    void increment() {
      value = value + step;
      value = value > 127 ? 127 : value;
    }
    void decrement() {
      value = value - step;
      value = (value < 0) ? 0 : value;
    }
    void toggleMute() {
      this->mute = !mute;
    }
    void muteOn() {
      this->mute = true;
    }
    void muteOff() {
      this->mute = false;
    }
    bool getMute() {
      return mute;
    }
    byte getccChannel() {
      return ccChannel;
    }
    int getValue() {
      return value;
    }
    bool getNag() {
      return nag;
    }
    void sendMidi() {
      // MIDI.sendControlChange(ccChannel, getMidiValue(), 3);
    }
};