// плавный включатель-выключатель эффектов
void stepFader(bool dir, bool action) {
  // dir 0 up, 1 down
  // action 0 starting, 1 finishing
  // 0 0
  // 0 1
  // 1 0
  // 1 1
  byte mode = action | (dir << 1);
  int counter = 0;
  while (1) {
    if (curEffect == SNAKE) {
      EVERY_MS(LED_SPEED) {
        switch (mode) {
              case 0: rainbowDots(0, counter); break;
              case 1: rainbowDots(counter, NUMLEDS); break;
              case 2: rainbowDots(NUMLEDS - counter, NUMLEDS); break;
              case 3: rainbowDots(0, NUMLEDS - counter); break;
        }
        counter++;
        yield();
        Serial.println("rainbow!");
        Serial.print(counter);
        if (counter == NUMLEDS) {
          Serial.println("Quitting rainbow");
          break;
          
        }
      }
    } else {
      EVERY_MS(STEP_SPEED) {
        counter++;
        switch (curEffect) {
          case COLOR:
            switch (mode) {
              case 0: staticColor(1, 0, counter); break;
              case 1: staticColor(1, counter, STEP_AMOUNT); break;
              case 2: staticColor(-1, STEP_AMOUNT - counter, STEP_AMOUNT); break;
              case 3: staticColor(-1, 0, STEP_AMOUNT - counter); break;
            }
            break;
          case RAINBOW:
            switch (mode) {
              case 0: rainbowStripes(-1, STEP_AMOUNT - counter, STEP_AMOUNT); break;
              case 1: rainbowStripes(-1, 0, STEP_AMOUNT - counter); break;
              case 2: rainbowStripes(1, STEP_AMOUNT - counter, STEP_AMOUNT); break;
              case 3: rainbowStripes(1, 0, STEP_AMOUNT - counter); break;
            }
            break;
        }
        FastLED.show();
        if (counter == STEP_AMOUNT) break;
      }
    yield();
    }
  }
  if (action == 1) {
    FastLED.clear();
    FastLED.show();
  }
}

// ========= dots радужные
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
