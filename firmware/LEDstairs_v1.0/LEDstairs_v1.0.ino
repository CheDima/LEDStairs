
#include <FastLED.h>
#include "bitmaps.h"

#include "display.h"

#define STEP_AMOUNT 16     // количество ступенек
#define STEP_LENGTH 16    // количество чипов WS2811 на ступеньку

#define AUTO_BRIGHT 0     // автояркость 0/1 вкл/выкл (с фоторезистором)
#define CUSTOM_BRIGHT 60  // ручная яркость
#define NIGHT_BRIGHT 10

#define LED_SPEED 20      // ms for one led in runnig modes
#define STEP_SPEED 300    // ms for one step
#define START_EFFECT NIGHT   
#define ROTATE_EFFECTS 0      // 0/1 - автосмена эффектов
#define TIMEOUT 5            // секунд, таймаут выключения ступенек, если не сработал конечный датчик

// D1 and D2 are reserved for OLED 64x48 wemos D1 mini shield
#define SENSOR_UP D3
#define SENSOR_DOWN D6
#define STRIP_PIN D5
#define PHOTO_PIN A0
#define BUTTON_POWER D7
#define BUTTON_EFFECT D8

// для разработчиков
#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEPTH 2   // цветовая глубина: 1, 2, 3 (в байтах)

#define NUMLEDS STEP_AMOUNT * STEP_LENGTH // кол-во светодиодов
CRGB leds[NUMLEDS];  // буфер ленты

byte curBright = CUSTOM_BRIGHT;

enum DIRECTION {UP, DOWN};
enum ACTION {STARTING, FINISHING};
enum STATE {OFF, IDLE, WORK, WAITING_FOR_FINISH};

struct DisplayStatus {
  
};

struct Status {
  DIRECTION direction;
  ACTION action;
  STATE state;
};

Status systemState;


enum EFFECTS {SNAKE, NIGHT};
EFFECTS curEffect;
#define EFFECTS_AMOUNT 2

// ==== удобные макросы ====
#define FOR_i(from, to) for(int i = (from); i < (to); i++)
#define FOR_j(from, to) for(int j = (from); j < (to); j++)
#define FOR_k(from, to) for(int k = (from); k < (to); k++)
#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr = millis();\
  if (flag)
//===========================
#define IS_PRESSED(btn) digitalRead(btn) == LOW 

byte stepCount = 0;
int ledCount = 0;
uint32_t idleTimeoutCounter;
    
CRGBPalette16 firePalette;
Display disp;

void setup() {
  FastLED.addLeds<WS2812, STRIP_PIN, GRB>(leds, NUMLEDS).setCorrection( TypicalLEDStrip );
  Serial.begin(9600);
  FastLED.setBrightness(curBright);    // brightness (0-255)
  pinMode(SENSOR_UP, INPUT);
  pinMode(SENSOR_DOWN, INPUT);
  pinMode(BUTTON_POWER, INPUT_PULLUP);

  delay(100);
  FastLED.clear();
  FastLED.show();
  turnOn();
}

void loop() {
  if (!systemIs(OFF)) {
    getBright();
    readSensors();
    updateEffects();
  }
  readButtons();
  updateDisplay();
}

void turnOn() {
  disp.scrollText("Hi");
  curEffect = START_EFFECT;
  systemState.state = IDLE;
}

void turnOff() {
  systemState.state = OFF;
}

void readButtons() {
  if (IS_PRESSED(BUTTON_POWER)) {
    if (systemIs(OFF)) {
      turnOn();
    } else {
      turnOff();
    }
  }

  if (IS_PRESSED(BUTTON_EFFECT)) {

  }

}

void updateDisplay() {
  
}

bool systemIs(STATE state) {
    return state == systemState.state;
}

bool systemIs(STATE state, DIRECTION dir, ACTION action = STARTING) {
    return state == systemState.state && dir == systemState.direction && action == systemState.action;
}

void changeState(DIRECTION dir, ACTION action = STARTING, STATE state = WORK) {
  systemState.direction = dir;
  systemState.action = action;
  systemState.state = state;
  stepCount = 0;
  ledCount = 0;
}

void readSensors() {

  fadeIfTimeOut();

  // Action

  EVERY_MS(50) {
    if (digitalRead(SENSOR_UP)) {
      if (systemIs(IDLE)) {
        changeState(DOWN);
      } else 
      if (systemIs(WAITING_FOR_FINISH, UP)){
        changeState(UP, FINISHING);
      }
    }

    if (digitalRead(SENSOR_DOWN)) {
      if (systemIs(IDLE)) {
        changeState(UP);
      } else 
      if (systemIs(WAITING_FOR_FINISH, DOWN)){
        changeState(DOWN, FINISHING);
      }
    }
  }
}

void getBright() {
#if (AUTO_BRIGHT == 1)
  if (systemState == IDLE) {  // в режиме простоя
    EVERY_MS(3000) {            // каждые 3 сек
      //Serial.println(analogRead(PHOTO_PIN));
      curBright = map(analogRead(PHOTO_PIN), 30, 800, 10, 200);
      FastLED.setBrightness(curBright);
    }
  }
#endif
}
