static const unsigned char PROGMEM star_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

static const unsigned char PROGMEM moon_bmp[] = 
{
  0x01, 0x00, 
  0x0e, 0x04, 
  0x18, 0x3c, 
  0x30, 0x3c, 
  0x70, 0x3f, 
  0x60, 0x7c, 
  0xe0, 0x08, 
  0xe0, 0x08, 
  0xe0, 0x00, 
  0xf0, 0x02, 
  0xf0, 0x06, 
  0x78, 0x0c, 
  0x7e, 0x1c, 
  0x3f, 0xf8, 
  0x1f, 0xf0, 
  0x0f, 0xe0, 
};

static const unsigned char PROGMEM cat_64x43_bmp[] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xf8, 0x00, 0x00, 0x1f, 0x00, 0x00, 
  0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0xc0, 0x00, 
  0x00, 0x03, 0xfe, 0x00, 0x00, 0x7f, 0xc0, 0x00, 
  0x00, 0x4f, 0xc7, 0x00, 0x00, 0xc3, 0xe6, 0x00, 
  0x00, 0x1f, 0xc7, 0x80, 0x03, 0xc3, 0xf8, 0x00, 
  0x00, 0x1f, 0xc7, 0xc0, 0x03, 0xc3, 0xf8, 0x00, 
  0x02, 0x0f, 0xc7, 0xc0, 0x03, 0xc3, 0xe0, 0xc0, 
  0x02, 0x0f, 0xc7, 0xc0, 0x07, 0xc3, 0xf0, 0xc0, 
  0x02, 0x0f, 0xc7, 0xe0, 0x0f, 0xc3, 0xf0, 0xc0, 
  0x02, 0x0f, 0xc7, 0xe0, 0x07, 0xc3, 0xe0, 0xc0, 
  0x02, 0x07, 0xc7, 0xe0, 0x0f, 0xe3, 0xc0, 0xc0, 
  0x02, 0x03, 0xcf, 0xe0, 0x0f, 0xf3, 0xc0, 0xc0, 
  0x07, 0x81, 0xff, 0xe0, 0x07, 0xff, 0x01, 0xe0, 
  0x07, 0x80, 0x3f, 0xe0, 0x0f, 0xfc, 0x01, 0xe0, 
  0x07, 0x80, 0x3f, 0xe0, 0x07, 0xf8, 0x01, 0xe0, 
  0x06, 0x40, 0x00, 0x20, 0x04, 0x00, 0x06, 0xe0, 
  0x03, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0xc0, 
  0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 
  0x03, 0xcc, 0x00, 0x1f, 0xf0, 0x00, 0x37, 0xc0, 
  0x03, 0xec, 0x00, 0x0f, 0xe0, 0x00, 0x37, 0x80, 
  0x00, 0xfe, 0x00, 0x07, 0xe0, 0x00, 0x7f, 0x00, 
  0x00, 0xf3, 0xc0, 0x03, 0x80, 0x03, 0xcf, 0x00, 
  0x00, 0xf7, 0xc0, 0x03, 0xc0, 0x07, 0xde, 0x00, 
  0x00, 0x7e, 0x7e, 0x07, 0xe0, 0x7e, 0x7e, 0x00, 
  0x00, 0x13, 0xc9, 0xff, 0xff, 0x93, 0xc8, 0x00, 
  0x00, 0x13, 0xc9, 0xff, 0xff, 0x97, 0xc8, 0x00, 
  0x00, 0x0e, 0x79, 0x23, 0x84, 0x9e, 0x60, 0x00, 
  0x00, 0x03, 0xcf, 0x27, 0xec, 0xf3, 0xc0, 0x00, 
  0x00, 0x03, 0xcf, 0x27, 0xed, 0xf3, 0xc0, 0x00, 
  0x00, 0x00, 0x79, 0xfc, 0xff, 0x9e, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xe4, 0x93, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xe4, 0x93, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
