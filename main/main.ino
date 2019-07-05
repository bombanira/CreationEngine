
// tft 
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> /* must modify private to protected */
#include <SPI.h>

#define TFT_CS     10
#define TFT_RST    -1
#define TFT_DC     8
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
TFTST7735 tft = TFTST7735(TFT_CS, TFT_DC, TFT_RST);

// 7seg send
void send_7seg(char *data);
void now_menu_mode_display(void);

// colorSensor librarys
#include <Wire.h>
#include <S11059.h>
#include <EEPROM.h>

#define contiRedMode 0
#define contiGreenMode 1
#define contiBlueMode 2

// スイッチ
#define UpButton 2
#define DownButton 3
#define DetermineButton 4
bool pres1 = false, pres2 = false, pres3 = false, double_button = false;
enum menu_mode {color_constant, color_series, recolor_constant, recolor_series, option};
bool menu_determinate = false;
enum menu_mode now_menu_mode = color_constant;
enum get_option_mode {absolute, relative};
enum get_option_mode now_option_mode = absolute;
bool get_option_determinate = false;
void now_menu_mode_change(byte num);
void get_option_determinate_change(void);
unsigned long t, start_time;

// color 
S11059 colorSensor;
uint16_t abs_max_red, abs_max_green, abs_max_blue;
uint16_t rel_max_red, rel_max_green, rel_max_blue;
bool saved_value, relative_get = false;

// #### prototype declare #### 
uint8_t RGBCount(uint16_t color, float coeffi);
uint16_t readEEPROM(byte colorKind);
void writeEEPROM(byte colorKind, uint16_t data);
void color_setup(int time_mode, int set_time);
void colorSensor_setup(void);
void color_setup(int time_mode, int set_time)
{
  // 積分モードをマニュアル設定モードに設定
  colorSensor.setMode(S11059_MODE_MANUAL);

  // ゲイン選択
  // * S11059_GAIN_HIGH: Highゲイン
  // * S11059_GAIN_LOW: Lowゲイン
  colorSensor.setGain(S11059_GAIN_HIGH);

  // 1色あたりの積分時間設定(下記は指定する定数とマニュアル設定モードの場合の積分時間)
  // * S11059_TINT0: 175 us
  // * S11059_TINT1: 2.8 ms
  // * S11059_TINT2: 44.8 ms
  // * S11059_TINT3: 358.4 ms
  switch(time_mode)
  {
    case 0:
      colorSensor.setTint(S11059_TINT0);
      break;
    case 1:
      colorSensor.setTint(S11059_TINT1);
      break;
    case 2:
      colorSensor.setTint(S11059_TINT1);
      break;
    case 3:
      colorSensor.setTint(S11059_TINT1);
      break;
    default:
      Serial.println("argument exception in color_setup() ");
      delay(5000);
      break;
  }
  
  // マニュアルタイミング(0~65535)
  // setTint()で指定した積分時間を何倍まで長くするかを設定
  colorSensor.setManualTiming(set_time);
}
void setup() {
  pinMode(UpButton, INPUT);
  pinMode(DownButton, INPUT);
  pinMode(DetermineButton, INPUT);
  // tft
  tft.init();
  tft.fillScreen(ST7735_BLACK);
  // tft end
  Serial.begin(9600);
  // colorSensor.begin();   // S11059#begin()はライブラリのバージョン0.1.0で除去されました
  Wire.begin();              // 直接Wire.begin()を呼び出してください (2017/05/01 追記)
  color_setup(1, 12);
  // 白認識をしているか
  // ボタン
  
  if(EEPROM.read(0) != 0)
  {
    Serial.println("setup: data in EEPROM");
    saved_value = true;
    abs_max_red = readEEPROM(contiRedMode);
    abs_max_green = readEEPROM(contiGreenMode);
    abs_max_blue = readEEPROM(contiBlueMode);
  }
  else
  {
    Serial.println("setup read EEPROM");
    saved_value = false;
  } 
  Serial.println("setup() end");
  start_time = millis();
  //saved_value = false;
  send_7seg("COLCON");
}

void loop() {
  
  // 積分時間よりも長く待機
  //
  // マニュアル設定モード時のS11059.delay()で実行される待機処理の時間は
  // S11059.setTint(tint)、S11059.setManualTiming(manualTiming)で
  // 設定した値に応じて異なります
//  if(digitalRead(UpButton) && digitalRead(DownButton) && pres4 != digitalRead(DownButton))
//  {
//    Serial.println("doubleButton");
//
//    pres4 = true; 
//  }
//  else if(!digitalRead(UpButton) && !digitalRead(DownButton))
//    pres4 = false;
//  
  if(digitalRead(UpButton) && pres1 != digitalRead(UpButton))
  {
    t = millis();
    while(millis() -t < 200)
    {
      if(digitalRead(DownButton))
      {
        pres2 = true;
        double_button = true;
        if(get_option_determinate)
        {
          get_option_determinate = false;
          get_option_determinate_display();
        }
        else
        {
          menu_determinate = false;
          now_menu_mode_display();
        }
        Serial.println("doubleButton");
        break;
      }
    }
    if(!double_button)
    {
      Serial.println("Upbutton");
      if(menu_determinate)
      {
        get_option_determinate_change();        
      }
      else
      {
        now_menu_mode_change(-1);        
      }
    }
    pres1 = true;
  }
  else if(!digitalRead(DownButton) && !digitalRead(UpButton) && double_button)
    double_button = false;
  else if(!digitalRead(UpButton))
    pres1 = false;
  
  if(digitalRead(DownButton) && pres2 != digitalRead(DownButton))
  {
    t = millis();
    while(millis() -t < 200)
    {
      if(digitalRead(UpButton))
      {
        pres1 = true;
        double_button = true;
        if(get_option_determinate)
        {
          get_option_determinate = false;
          get_option_determinate_display();
        }
        else
        {
          menu_determinate = false;
          now_menu_mode_display();
        }
        Serial.println("doubleButton");
        break;
      }
    }
    if(!double_button)
    {
      Serial.println("Downbutton");
      if(menu_determinate)
      {
        get_option_determinate_change();        
      }
      else
      {
        now_menu_mode_change(1);        
      }
    }
    pres2 = true;
  }
  else if(!digitalRead(DownButton) && !digitalRead(UpButton) && double_button)
    double_button = false;
  else if(!digitalRead(DownButton))
    pres2 = false;
  
  if(digitalRead(DetermineButton) && pres3 != digitalRead(DetermineButton))
  {
    Serial.println("Determinebutton");
    if(menu_determinate)
    {
      get_option_determinate = true;
      send_7seg("000000");
    }
    else
    {
      menu_determinate = true;
      get_option_determinate_display();
    }
    pres3 = true; 
  }
  else if(!digitalRead(DetermineButton))
    pres3 = false;
    
  if(Serial.available() > 0)
  {
    char input = Serial.read();

    if(input == 'a')
    {
      colorSensor_setup();
      colorSensor.delay();
          // 赤、緑、青、赤外データをリード
      if (colorSensor.update()) 
      {
        uint16_t red = colorSensor.getRed();
        uint16_t green = colorSensor.getGreen();
        uint16_t blue = colorSensor.getBlue();
        uint8_t red_8 = RGBCount(red, abs_max_red);
        uint8_t green_8 = RGBCount(green, abs_max_green);
        uint8_t blue_8 = RGBCount(blue, abs_max_blue);
        tft.fillScreen(tft.color565(blue_8, green_8, red_8));
        //tft.fillScreen((tft.color565(255, 255, 255)));
        tft.endWrite();
        Serial.println("");
        if(!saved_value)
        {
          writeEEPROM(contiRedMode,red);
          writeEEPROM(contiGreenMode, green);
          writeEEPROM(contiBlueMode, blue);
          abs_max_red = red;
          abs_max_green = green;
          abs_max_blue = blue;
          saved_value = true;
        }
      }
      else 
      {
        Serial.println("update failed");
      }
    }
    else if(input == 'r')
    {
      colorSensor_setup();
      colorSensor.delay();
      if(colorSensor.update())
      {
        rel_max_red = colorSensor.getRed();
        rel_max_green = colorSensor.getGreen();
        rel_max_blue = colorSensor.getBlue();
        RGBCount(rel_max_red, 0);
        RGBCount(rel_max_green, 0);
        RGBCount(rel_max_blue, 0);
        Serial.println("relative light got !!");
        Serial.print("red is  ");
        Serial.println(rel_max_red);

        delay(5000);
        colorSensor_setup();
        colorSensor.delay();
        if(colorSensor.update())
        {
          uint8_t red_8 = RGBCount(colorSensor.getRed(), abs_max_red);
          uint8_t green_8 = RGBCount(colorSensor.getGreen(), abs_max_green);
          uint8_t blue_8 = RGBCount(colorSensor.getBlue(), abs_max_blue);
          tft.fillScreen(tft.color565(blue_8, green_8, red_8));
        }
      }
    }
    else if(input == 'n')
    {
      colorSensor_setup();
      colorSensor.delay();
      if(colorSensor.update())
      {
        uint16_t red = colorSensor.getRed();
        uint16_t green = colorSensor.getGreen();
        uint16_t blue = colorSensor.getBlue();
        writeEEPROM(contiRedMode,red);
        writeEEPROM(contiGreenMode, green);
        writeEEPROM(contiBlueMode, blue);
        abs_max_red = red;
        abs_max_green = green;
        abs_max_blue = blue;
      }
    }
  }
}

void now_menu_mode_change(int num)
{
  if(num == -1 && now_menu_mode == 0)
    now_menu_mode = option;
  else if(num == 1 && now_menu_mode == 4)
    now_menu_mode = color_constant;
  else
    now_menu_mode = now_menu_mode + num;
  now_menu_mode_display();
}
void now_menu_mode_display(void)
{
  switch(now_menu_mode)
  {
    case color_constant:
      send_7seg("COLCON");
      break;
    case color_series:
      send_7seg("COLSER");
      break;
    case recolor_constant:
      send_7seg("RE_CON");
      break;
    case recolor_series:
      send_7seg("RE_SER");
      break;
    case option:
      send_7seg("CONF__");
      break;
  }
  Serial.print("now_menu_mode is ");
  Serial.println(now_menu_mode);
}
void get_option_determinate_change(void)
{
  get_option_determinate = (get_option_determinate + 1) % 2;
  get_option_determinate_display();
}
void get_option_determinate_display(void)
{
  switch(get_option_determinate)
  {
    case absolute:
      send_7seg("ABSOL ");
      break;
    case relative:
      send_7seg("RELAT ");
      break;
  }
  Serial.print("get_option_determinate is ");
  Serial.println(get_option_determinate);
}
void send_7seg(char *data)
{
  Wire.beginTransmission(8);
  Serial.println(data);
  Wire.write(data);
  byte wire_return = Wire.endTransmission();
  Serial.println("wire_return is ");
  Serial.println(wire_return);
  switch(wire_return)
  {
    case 0:
      Serial.println("I2C succsess!!");
      break;
    case 1:
      Serial.println("buffer size over");
      break;
  }
}
uint16_t readEEPROM(byte colorKind)
{
  Serial.println("readEEPROM  reading color");
  byte mostBit = colorKind * 2, leastBit = mostBit + 1;
  uint16_t color;
  if(colorKind < 0 && 2 < colorKind) // mode error
  {
    Serial.println("readEEPROM invalid argument error ");
    delay(5000);
    return 0;
  }
  color = EEPROM.read(mostBit);
  Serial.print("most value is ");
  Serial.println(color);
  color = color << 8;
  Serial.print("least value is ");
  Serial.println(EEPROM.read(leastBit));
  color |= EEPROM.read(leastBit);
  Serial.print("return color value is ");
  Serial.println(color);
  Serial.println("");
  return color;
}
void writeEEPROM(byte colorKind, uint16_t data)
{
  Serial.println("writeEEPROM");
  Serial.print("value is ");
  Serial.println(data);
  byte mostBit = colorKind * 2, leastBit = mostBit + 1;
  if(colorKind < 0 && 2 < colorKind) // mode error
  {
    Serial.println("readEEPROM invalid argument error");
    delay(5000);
  }
  EEPROM.write(mostBit, highByte(data));
  Serial.print("most value is ");
  Serial.println(highByte(data));
  Serial.print("least value is ");
  Serial.println(lowByte(data));
  EEPROM.write(leastBit, lowByte(data));
}
uint8_t RGBCount(uint16_t color, float coeffi)
{
  Serial.print("default coeffi is ");
  Serial.println(coeffi);
  if(coeffi == 0)
    color = color >> 8;
  Serial.print("after coeffi is ");
  Serial.println(coeffi);
  Serial.print("default color is ");
  Serial.println(color);
  Serial.print("adjust color is ");
  uint8_t tmp = color/coeffi * 255 + 0.5;
  Serial.println(tmp, HEX);
  Serial.println("");
  return tmp;
}
void colorSensor_setup(void)
{
    // ADCリセット、スリープ解除
  if (!colorSensor.reset()) {
    Serial.println("reset failed");
  }

  // ADCリセット解除、バスリリース
  if (!colorSensor.start()) { 
    Serial.println("start failed");
  }
}
