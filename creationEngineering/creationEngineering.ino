
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> /* must modify private to protected */
#include <SPI.h>

/*  about programs information, check this program https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp 
    and available function list http://www.humblesoft.com/wiki/?Adafruit_GFX%E3%81%AE%E3%83%A1%E3%82%BD%E3%83%83%E3%83%89
*/


class TFTST7735 : public Adafruit_ST7735 {
public:
  TFTST7735(int8_t CS, int8_t RS, int8_t RST = -1) :
  Adafruit_ST7735(CS, RS, RST)
  {
  }

  void init() 
  {
    initR(-1);

    _height = 160;
    _rowstart = 80;
    _colstart = 0;
  }
};

#define TFT_CS     10
#define TFT_RST    -1
#define TFT_DC     8
#define Voltage 7
TFTST7735 tft = TFTST7735(TFT_CS, TFT_DC, TFT_RST);
uint16_t red = 0x00, blue = 0x00, green = 0x00;

void setup (void) {
  // ##### TFT test #####
  tft.init();
  tft.fillScreen(ST7735_BLACK);
  pinMode(Voltage, OUTPUT);
  digitalWrite(Voltage, HIGH);
  // ##### TFT test #####
}

/*
void loop() {
  tft.setTextWrap(false);

  // font size is 5x7
  tft.setCursor(5, 5); // set lcd cursor position
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.fillRect(5, 5, 128, 7, ST7735_BLACK);
  tft.println(millis());

  tft.setCursor(5, 20);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.fillRect(5, 20, 128, 7*2, ST7735_BLACK);
  tft.println(millis());
  delay(24);
}
*/
void loop()
{
  // ##### TFT test #####
  for(int i=0; i < 255; i++)
  {
    red = blue = green = i;
    tft.fillScreen(convert_RGB(red, blue, green));
  }
  // ##### TFT test #####
}
uint16_t convert_RGB(uint16_t red, uint16_t blue, uint16_t green)
{
  uint16_t RGB = red;
  RGB << 8;
  RGB += blue;
  RGB << 8;
  RGB += green;
  return RGB;
}
