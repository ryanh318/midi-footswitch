#include <Arduino.h>
#include <U8g2lib.h>
#include <MIDI.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/A5, /* data=*/A4, /* reset=*/U8X8_PIN_NONE);  // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
MIDI_CREATE_DEFAULT_INSTANCE();
#include "Level.h"
#include "Led.h"
#include "Button.h"

const byte BUTTON_NUM = 4;
const byte BUTTON_LEVEL_NUM = 12;
const byte LED_NUM = 6;
const byte LED_LEVEL_NUM = 7;
const byte LEVEL_NUM = 8;


Led leds[LED_NUM] = {Led(2), Led(3), Led(4), Led(5), Led(6), Led(7)};
Button buttons[BUTTON_NUM] = {Button(A0), Button(A1), Button(A2), Button(A3)};

#include "ButtonState.h"
ButtonState buttonState = ButtonState();

Level levels[LEVEL_NUM] = {
  // Send
  Level(21, 100, true),
  // Output
  Level(22, true),
  // Drive
  Level(25, true),
  // Drive Fuzz
  Level(26, true),
  // Drive Oct Fuzz
  Level(27, true),
  // Octave
  Level(28, true),
  // Boost
  Level(29, true),
  // Tuner
  Level(68, 127, false, false),
};

#include "ButtonLevel.h"
#include "LedLevel.h"
#include "LevelScreen.h"

ButtonLevel buttonLevels[BUTTON_LEVEL_NUM] = {
  // Volume up 
  ButtonLevel(0, 0, 0, INCREMENT, PRESS_AND_RELEASE), 
  ButtonLevel(0, 0, 0, INCREMENT, DOUBLE_PRESS_AND_HOLD), 
  //Volume down
  ButtonLevel(1, 1, 0, DECREMENT, PRESS_AND_RELEASE), 
  ButtonLevel(1, 1, 0, DECREMENT, DOUBLE_PRESS_AND_HOLD), 
  // Mute Send
  ButtonLevel(1, 0, 0, TOGGLE_MUTE, SHIFT_PRESS_AND_RELEASE), 
  // Mute Out
  ButtonLevel(0, 1, 1, TOGGLE_MUTE, SHIFT_PRESS_AND_RELEASE), 
  // TUNER
  ButtonLevel(1, 0, 9, UNMUTE, SHIFT_PRESS_AND_HOLD_TOGGLE), 
  // Drive
  ButtonLevel(3, 3, 4, TOGGLE_MUTE, PRESS_AND_RELEASE), 
  // Drive Fuzz
  ButtonLevel(2, 3, 5, TOGGLE_MUTE, SHIFT_PRESS_AND_RELEASE), 
  // Drive Fuzz Octave
  ButtonLevel(2, 3, 6, TOGGLE_MUTE, SHIFT_PRESS_AND_HOLD_TOGGLE), 
  // Octave
  ButtonLevel(2, 2, 7, TOGGLE_MUTE, PRESS_AND_RELEASE),
  // Boost
  ButtonLevel(3, 2, 8, TOGGLE_MUTE, SHIFT_PRESS_AND_RELEASE),

};

LedLevel ledLevels[LED_LEVEL_NUM] = {
  // Mute Send
  LedLevel(0, 0, STATIC),
  // Mute Out
  LedLevel(1, 1, STATIC),
  // Boost
  LedLevel(4, 8, STATIC),
  // Octave
  LedLevel(5, 7, STATIC),
  // Drive
  LedLevel(6, 1, STATIC),
  // Drive Fuzz
  LedLevel(7, 5, STATIC),
  // Drive Fuzz Octave 
  LedLevel(7, 6, BLINK),
};

LevelScreen levelScreen = LevelScreen(0);

void setup() {
  Serial.begin(9600);
  Serial.println("hello");

  u8g2.begin();

  // MIDI.begin(MIDI_CHANNEL_OFF); 
}

void loop() {
  scanButtons();
  drawLeds();
  drawLevels();
  // nag();
}

void scanButtons() {
  buttonState.update();
  for(int i=0;i<BUTTON_LEVEL_NUM;i++) {
    buttonLevels[i].checkAndActivate();
  }
}

void drawLeds() {
  for(int i=0;i<LED_LEVEL_NUM;i++) 
    ledLevels[i].setLedState();

  for(int i=0;i<LED_NUM;i++) 
    leds[i].display();
}

void drawLevels() {
  drawScreen();
}

void nag() {
  unsigned long time = buttonState.getLastTime() > lastNagTime ? buttonState.getLastTime() : lastNagTime;
  if(millis() - time  > 5000) {
    Serial.println("nag");
    lastNagTime = millis();
    for(int i=0;i<LEVEL_NUM;i++) {
      if(levels[i].getNag())
        levels[i].sendMidi();
    }
  }
}

void drawScreen() {
  if(lastScreenDraw == buttonState.getLastTime()) return;
  if(millis() - buttonState.getLastTime() < 1000) return;
  Level level = levels[levelScreen.getLevel()];
  if(lastScreenLevel == level.getValue()) return;

  u8g2.clearBuffer();
  u8g2.setFont(u8g_font_profont29r);
  u8g2.setCursor(76, 32);
  u8g2.print(level.getValue());
  u8g2.sendBuffer();
  lastScreenDraw = buttonState.getLastTime();
  lastScreenLevel = level.getValue();
}
