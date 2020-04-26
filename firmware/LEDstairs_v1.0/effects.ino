
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUMLEDS) ] += CRGB::White;
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
