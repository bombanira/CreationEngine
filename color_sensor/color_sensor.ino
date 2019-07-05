#include <Wire.h>
#include <S11059.h>

//#define R_white
//#define G_white
//#define B_white
S11059 colorSensor;
// void RGBOut(uint16_t color, float count, int relation);
void RGBCount(uint16_t color, float count);
void setup() {
  Serial.begin(9600);
  // colorSensor.begin();   // S11059#begin()はライブラリのバージョン0.1.0で除去されました
  Wire.begin();              // 直接Wire.begin()を呼び出してください (2017/05/01 追記)

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
  colorSensor.setTint(S11059_TINT1);

  // マニュアルタイミング(0~65535)
  // setTint()で指定した積分時間を何倍まで長くするかを設定
  colorSensor.setManualTiming(12);
}

void loop() {
  // ADCリセット、スリープ解除
  if (!colorSensor.reset()) {
    Serial.println("reset failed");
  }

  // ADCリセット解除、バスリリース
  if (!colorSensor.start()) { 
    Serial.println("start failed");
  }

  // 積分時間よりも長く待機
  //
  // マニュアル設定モード時のS11059.delay()で実行される待機処理の時間は
  // S11059.setTint(tint)、S11059.setManualTiming(manualTiming)で
  // 設定した値に応じて異なります
  if (Serial.available() > 0)
  {
    colorSensor.delay();
    // 赤、緑、青、赤外データをリード
    if (colorSensor.update()) {
      uint16_t red = colorSensor.getRed();
      uint16_t green = colorSensor.getGreen();
      uint16_t blue = colorSensor.getBlue();
      Serial.print("red ");
      //RGBCount(red, 112, 100);
      // RGBCount(red, 112);
      RGBCount(red, (6.89*9511)/255);
      Serial.print("green ");
      //RGBCount(green, 83.5, 73);
      RGBCount(green, 83.5);
      Serial.print("blue ");
      RGBCount(blue, 44);
      //RGBCount(blue, 44, 47);
      Serial.println("");
    } else {
      Serial.println("update failed");
    }
  }

  delay(5000);
}
void RGBCount(uint16_t color, float count)
{
  Serial.print("default color is ");
  Serial.println(color);
  Serial.print("adjust color is ");
  Serial.println(color * count);
}
//void RGBCount(uint16_t color, float count, int relation)
//{
//  Serial.print("default color is ");
//  Serial.println(color);
//  Serial.print("adjust color is ");
//  Serial.println(color/count*100/relation);
//}
