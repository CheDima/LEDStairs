/*
  Скетч к проекту "Подсветка лестницы"
  Страница проекта (схемы, описания): https://alexgyver.ru/ledstairs/
  Исходники на GitHub: https://github.com/AlexGyver/LEDstairs
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

#include <FastLED.h>

#define STEP_AMOUNT 16     // количество ступенек
#define STEP_LENGTH 16    // количество чипов WS2811 на ступеньку

#define AUTO_BRIGHT 0     // автояркость 0/1 вкл/выкл (с фоторезистором)
#define CUSTOM_BRIGHT 40  // ручная яркость

#define LED_SPEED 20      // ms for one led in runnig modes
#define STEP_SPEED 300    // ms for one step
#define START_EFFECT SNAKE    // режим при старте COLOR, RAINBOW, FIRE
#define ROTATE_EFFECTS 0      // 0/1 - автосмена эффектов
#define TIMEOUT 5            // секунд, таймаут выключения ступенек, если не сработал конечный датчик

// пины
// если перепутаны сенсоры - можно поменять их местами в коде! Вот тут
#define SENSOR_UP D3
#define SENSOR_DOWN D2
#define STRIP_PIN D5    // пин ленты 
#define PHOTO_PIN A0

// для разработчиков
#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEPTH 2   // цветовая глубина: 1, 2, 3 (в байтах)

#define NUMLEDS STEP_AMOUNT * STEP_LENGTH // кол-во светодиодов
CRGB leds[NUMLEDS];  // буфер ленты

byte curBright = CUSTOM_BRIGHT;

enum DIRECTION {UP, DOWN};
enum ACTION {STARTING, FINISHING};
enum STATE {IDLE, WORK, WAITING_FOR_FINISH};

struct Status {
  DIRECTION direction;
  ACTION action;
  STATE state;
};

Status systemState;


enum EFFECTS {COLOR, RAINBOW, NIGHT, SNAKE};
EFFECTS curEffect = START_EFFECT;
#define EFFECTS_AMOUNT 3
byte effectCounter;

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


byte stepCount = 0;
int ledCount = 0;
uint32_t idleTimeoutCounter;
    
CRGBPalette16 firePalette;

void setup() {
  FastLED.addLeds<WS2812, STRIP_PIN, GRB>(leds, NUMLEDS).setCorrection( TypicalLEDStrip );
  Serial.begin(9600);
  FastLED.setBrightness(curBright);    // brightness (0-255)
  pinMode(SENSOR_UP, INPUT);
  pinMode(SENSOR_DOWN, INPUT);
  // для кнопок
  //pinMode(SENSOR_START, INPUT_PULLUP);
  //pinMode(SENSOR_END, INPUT_PULLUP);

  delay(100);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  getBright();
  readSensors();
  updateEffects();
}

bool systemIs(STATE state) {
    return state == systemState.state;
}

bool systemIs(STATE state, DIRECTION dir, ACTION action = STARTING) {
    return state == systemState.state && dir == systemState.direction && action == systemState.action;
}

void updateEffects() {
  if (systemState.state == WORK && curEffect == SNAKE) {
    EVERY_MS(LED_SPEED) {
      FastLED.clear();
      if (systemIs(WORK, UP, STARTING)) rainbowDots(0, ledCount);
      if (systemIs(WORK, UP, FINISHING)) rainbowDots(ledCount, NUMLEDS); 
      if (systemIs(WORK, DOWN, STARTING)) rainbowDots(NUMLEDS - ledCount, NUMLEDS);
      if (systemIs(WORK, DOWN, FINISHING)) rainbowDots(0, NUMLEDS - ledCount);
      ledCount++;
      if (ledCount == NUMLEDS) {
        if (systemState.action == FINISHING) {
          systemState.state = IDLE;
        } else {
          systemState.state = WAITING_FOR_FINISH;
          idleTimeoutCounter = millis();
        }
      }
      FastLED.show();
    }

  }
}

void fadeIfTimeOut() {
  // fade if time out

  if (systemIs(WAITING_FOR_FINISH) && millis() - idleTimeoutCounter >= (TIMEOUT * 1000L)) {
    Serial.println("Entered fadeIfTimeOut");
    systemState.state = IDLE;
    int changeBright = curBright;
    while (1) {
      EVERY_MS(50) {
        changeBright -= 5;
        if (changeBright < 0) {
          break;
        }
        FastLED.setBrightness(changeBright);
        FastLED.show();
      }
    }
    FastLED.clear();
    FastLED.setBrightness(curBright);
    FastLED.show();
  }
  
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
