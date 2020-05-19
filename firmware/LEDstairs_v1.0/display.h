#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

class Display {
  public: 
  Display() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
    display.display();
    // Clear the buffer.
  
    display.clearDisplay();
    //scrollText("Good Day");
    display.clearDisplay();
    display.display();   
  }
  
  void showPicture() {
        showPicture(cat_64x43_bmp);
  }
  
  void showPicture(const uint8_t *bitmap) {
     display.drawBitmap(0, 0, bitmap, 64, 43, WHITE);
     display.display();
     delay(2000);
     dim(true);
     delay(2000);
     display.clearDisplay();
     display.display();
     delay(2000);
  }
  
  void dim(boolean isDim) {
    display.dim(isDim);
  }

  

  void scrollText(String text) {
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,0);
    display.clearDisplay();
    display.println(text);
    display.display();
    delay(1);
    display.startscrollleft(0x00, 0x0F);
    delay(2000);
    display.startscrollright(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
  
  }

  private:
  int startTime;

};
