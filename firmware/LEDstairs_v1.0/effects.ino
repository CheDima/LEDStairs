
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
    clearLed();
  }
  
}

void updateEffects() {
  if (curEffect == EFFECT_NIGHT) {
    rainbowDots(0, NUMLEDS-1);
    FastLED.setBrightness(NIGHT_BRIGHT);
    addGlitter(80);
    FastLED.show();
  } else if (curEffect == EFFECT_JUST_WHITE) {
     fill_solid( &(leds[0]), NUMLEDS, CRGB::White);
     FastLED.show();
  }  else if (curEffect == EFFECT_JUST_BLUE) {
     fill_solid( &(leds[0]), NUMLEDS, CRGB::Blue);
     FastLED.show();
  } else if (systemState.state == WORK && curEffect == EFFECT_SNAKE) {
    EVERY_MS(LED_SPEED) {
      FastLED.clear();
      if (systemIs(WORK, UP, STARTING)) rainbowDots(0, ledCount);
      if (systemIs(WORK, UP, FINISHING)) rainbowDots(ledCount, NUMLEDS); 
      if (systemIs(WORK, DOWN, STARTING)) rainbowDots(NUMLEDS - ledCount, NUMLEDS);
      if (systemIs(WORK, DOWN, FINISHING)) rainbowDots(0, NUMLEDS - ledCount);
      ledCount++;
      if (ledCount == NUMLEDS) {
        if (systemState.action == FINISHING) {
          clearLed();
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

void clearLed() {
    FastLED.clear();
    FastLED.setBrightness(curBright);
    delay(200);
    FastLED.show();
    PRINTS("LED cleared");
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUMLEDS) ] += CRGB::White;
  }
}

// ========= from - inclusive, to - exclusive
void rainbowDots(int from, int to) {
  fill_rainbow(&(leds[from]), to - from, 100, 1);
}

// ========= смена цвета общая
void staticColor(int8_t dir, byte from, byte to) {
  byte thisBright;
  static byte colorCounter = 0;
  colorCounter += 2;
  FOR_i(0, STEP_AMOUNT) {
    thisBright = 255;
    if (i < from || i >= to) thisBright = 0;
    fillStep(i, CHSV(colorCounter, 255, thisBright));
  }
}

// ========= полоски радужные
void rainbowStripes(int8_t dir, byte from, byte to) {
  static byte colorCounter = 0;
  colorCounter += 2;
  byte thisBright;
  FOR_i(0, STEP_AMOUNT) {
    thisBright = 255;
    if (i < from || i >= to) thisBright = 0;
    fillStep((dir > 0) ? (i) : (STEP_AMOUNT - 1 - i), CHSV(colorCounter + (float)i * 255 / STEP_AMOUNT, 255, thisBright));
  }
}

// ========= залить ступеньку цветом (служебное)
void fillStep(int8_t num, CRGB color) {
  if (num >= STEP_AMOUNT || num < 0) return;
  FOR_i(num * STEP_LENGTH, num * STEP_LENGTH + STEP_LENGTH) {
    leds[i] = color;
  }
}
