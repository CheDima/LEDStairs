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

#define SNAKE_SPEED 20    // ms for one led in runnig modes
#define FADR_SPEED 300    // ms for one step
#define START_EFFECT SNAKE    // режим при старте COLOR, RAINBOW, FIRE
#define ROTATE_EFFECTS 0      // 0/1 - автосмена эффектов
#define TIMEOUT 15            // секунд, таймаут выключения ступенек, если не сработал конечный датчик

// пины
// если перепутаны сенсоры - можно поменять их местами в коде! Вот тут
#define SENSOR_START 0
#define SENSOR_END 4
#define STRIP_PIN D5    // пин ленты 
#define PHOTO_PIN A0

// для разработчиков
#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 2   // цветовая глубина: 1, 2, 3 (в байтах)

#define NUMLEDS STEP_AMOUNT * STEP_LENGTH // кол-во светодиодов
CRGB leds[NUMLEDS];  // буфер ленты

int effSpeed;
int8_t effDir;
byte curBright = CUSTOM_BRIGHT;
enum {S_IDLE, S_WORK} systemState = S_IDLE;
enum EFFECTS {COLOR, RAINBOW, NIGHT, SNAKE} curEffect = START_EFFECT;
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


int counter = 0;
CRGBPalette16 firePalette;

void setup() {
  pinMode(SENSOR_START, INPUT);
  pinMode(SENSOR_END, INPUT);
  FastLED.addLeds<WS2812, STRIP_PIN, GRB>(leds, NUMLEDS).setCorrection( TypicalLEDStrip );
  Serial.begin(9600);
  FastLED.setBrightness(curBright);    // яркость (0-255)
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
}

void getBright() {
#if (AUTO_BRIGHT == 1)
  if (systemState == S_IDLE) {  // в режиме простоя
    EVERY_MS(3000) {            // каждые 3 сек
      //Serial.println(analogRead(PHOTO_PIN));
      curBright = map(analogRead(PHOTO_PIN), 30, 800, 10, 200);
      FastLED.setBrightness(curBright);
    }
  }
#endif
}

// читаем сенсоры
void readSensors() {
  static bool flag1 = false, flag2 = false;
  static uint32_t timeoutCounter;
  if (systemState == S_WORK && millis() - timeoutCounter >= (TIMEOUT * 1000L)) {
    systemState = S_IDLE;
    int changeBright = curBright;
    while (1) {
      EVERY_MS(50) {
        changeBright -= 5;
        if (changeBright < 0) break;
        FastLED.setBrightness(changeBright);
        FastLED.show();
      }
    }
    FastLED.clear();
    FastLED.setBrightness(curBright);
    FastLED.show();
  }

  EVERY_MS(50) {
    // СЕНСОР У НАЧАЛА ЛЕСТНИЦЫ
    if (digitalRead(SENSOR_START)) {
      if (!flag1) {
        flag1 = true;
        timeoutCounter = millis();
        if (systemState == S_IDLE) {
          effDir = 1;
          if (ROTATE_EFFECTS) {
            if (++effectCounter >= EFFECTS_AMOUNT) effectCounter = 0;
            curEffect = (EFFECTS)effectCounter;
          }
        }
        switch (systemState) {
          case S_IDLE: stepFader(0, 0); systemState = S_WORK; break;
          case S_WORK:
            if (effDir == -1) {
              stepFader(1, 1); systemState = S_IDLE;
              FastLED.clear(); FastLED.show(); return;
            } break;
        }
      }
    } else {
      if (flag1) flag1 = false;
    }

    // СЕНСОР У КОНЦА ЛЕСТНИЦЫ
    if (digitalRead(SENSOR_END)) {
      Serial.println("End sensor triggered");
      if (!flag2) {
        flag2 = true;
        timeoutCounter = millis();
        if (systemState == S_IDLE) {
          effDir = -1;
          if (ROTATE_EFFECTS) {
            if (++effectCounter >= EFFECTS_AMOUNT) effectCounter = 0;
            curEffect = (EFFECTS) effectCounter;
          }
        }
        switch (systemState) {
          case S_IDLE: 
            stepFader(1, 0); systemState = S_WORK; break;
          case S_WORK:
            if (effDir == 1) {
              stepFader(0, 1); systemState = S_IDLE;
              FastLED.clear(); FastLED.show(); return;
            } break;
        }
      }
    } else {
      if (flag2) flag2 = false;
    }
  }
}
