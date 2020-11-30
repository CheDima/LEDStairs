   
#include <FastLED.h>
#include "GyverButton.h"

#include "bitmaps.h"

 #include <MatrDisplay.h>

#define STEP_AMOUNT 16     // количество ступенек
#define STEP_LENGTH 16    // количество чипов WS2811 на ступеньку

#define AUTO_BRIGHT 0     // автояркость 0/1 вкл/выкл (с фоторезистором)
#define DEFAULT_BRIGHT 60  // ручная яркость
#define NIGHT_BRIGHT 10

#define LED_SPEED 20      // ms for one led in runnig modes
#define STEP_SPEED 300    // ms for one step
#define START_EFFECT 1   
#define ROTATE_EFFECTS 0      // 0/1 - автосмена эффектов
#define TIMEOUT 5            // секунд, таймаут выключения ступенек, если не сработал конечный датчик

// D1 and D2 are reserved for OLED 64x48 wemos D1 mini shield
#define SENSOR_UP D8
#define SENSOR_DOWN D6
#define STRIP_PIN D5
#define PHOTO_PIN A0
#define BUTTON_POWER D4
#define BUTTON_EFFECT D7 

// для разработчиков
#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEPTH 2   // цветовая глубина: 1, 2, 3 (в байтах)

#define NUMLEDS STEP_AMOUNT * STEP_LENGTH // кол-во светодиодов
CRGB leds[NUMLEDS];  // буфер ленты

byte curBright = DEFAULT_BRIGHT;

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


#define EFFECT_SNAKE 1
#define EFFECT_NIGHT 2
#define EFFECT_JUST_WHITE 3
#define EFFECT_JUST_BLUE 4
int curEffect;
#define EFFECTS_AMOUNT 4

// ==== удобные макросы ====
#define PRINTS(x) Serial.print(F(x))
#define FOR_i(from, to) for(int i = (from); i < (to); i++)
#define FOR_j(from, to) for(int j = (from); j < (to); j++)
#define FOR_k(from, to) for(int k = (from); k < (to); k++)
#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr = millis();\
  if (flag)
//===========================

byte stepCount = 0;
int ledCount = 0;
uint32_t idleTimeoutCounter;
    
CRGBPalette16 firePalette;

MatrDisplay disp = MatrDisplay(new Oled64x48Display());

GButton effectButton(BUTTON_EFFECT, HIGH_PULL, NORM_OPEN);
GButton powerButton(BUTTON_POWER, HIGH_PULL, NORM_OPEN);
GButton sensorUp(SENSOR_UP, LOW_PULL, NORM_OPEN);
GButton sensorDown(SENSOR_DOWN, LOW_PULL, NORM_OPEN);

void setup() {
  FastLED.addLeds<WS2812, STRIP_PIN, GRB>(leds, NUMLEDS).setCorrection( TypicalLEDStrip );
  Serial.begin(9600);
  clearLed();
  delay(500);
  turnOn();
}

void loop() {
  if (!systemIs(OFF)) {
    //getBright();
    readSensors();
    updateEffects();
  }
  readButtons();
  tick();
}

void turnOn() {
  disp.runTempEffect(MatrEffect::sprite(cherry), 1000);
  curEffect = START_EFFECT;
  displayRedraw();
  systemState.state = IDLE;
  PRINTS("Power on");
}

void turnOff() {
  clearLed();
  disp.clear();
  disp.runTempEffect(MatrEffect::sprite(cat), 1000);
  systemState.state = OFF;
  PRINTS("Power off");
}

void readButtons() {
  if (powerButton.isClick()) {
    if (systemIs(OFF)) {
      turnOn();
    } else {
      turnOff();
    }
  }

  if (!systemIs(OFF) && effectButton.isClick()) {
    clearLed();
    if (curEffect == EFFECTS_AMOUNT) {
      curEffect = START_EFFECT;
    } else {
      curEffect++;
    }
    displayRedraw();
  }
}

void displayRedraw() {
  if (curEffect == EFFECT_SNAKE) {
     disp.setEffect(MatrEffect::sprite(rainbow));
  } else if (curEffect == EFFECT_NIGHT) {
    disp.setEffect(MatrEffect::sprite(moonStriped));
  } else if (curEffect == EFFECT_JUST_WHITE) {
    disp.setEffect(MatrEffect::sprite(moonWhite));
  } else if (curEffect == EFFECT_JUST_BLUE) {
    disp.setEffect(MatrEffect::sprite(moon));
  }
}

void tick() {
    disp.tick();
    effectButton.tick();
    powerButton.tick();
    sensorUp.tick();
    sensorDown.tick();
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

    if (sensorUp.isHold()) {
        PRINTS("Sensor up triggered");
      if (systemIs(IDLE)) {
        changeState(DOWN);
      } else 
      if (systemIs(WAITING_FOR_FINISH, UP)){
        changeState(UP, FINISHING);
      }
    }

    if (sensorDown.isHold()) {
      PRINTS("Sensor down triggered");
      if (systemIs(IDLE)) {
        changeState(UP);
      } else 
      if (systemIs(WAITING_FOR_FINISH, DOWN)){
        changeState(DOWN, FINISHING);
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
