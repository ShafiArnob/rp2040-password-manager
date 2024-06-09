#include <Arduino.h>
#include <WS2812FX.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#define LED_COUNT 1
#define LED_PIN 12

// WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
DynamicJsonDocument doc(1000); 

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.42" OLED

String password = "[{\"id\":1,\"name\":\"Gmail\"},{\"id\":2,\"name\":\"Facebook\"},{\"id\":3,\"name\":\"Github\"},{\"id\":4,\"name\":\"Instagram\"}]";

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void setup() {
  // put your setup code here, to run once:
  Wire.setSDA(22);
  Wire.setSCL(23);
  Wire.begin();  
  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer();
  u8g2_prepare();

  int render_menu = 0;

  deserializeJson(doc, password);
  for (JsonObject elem : doc.as<JsonArray>()) {
    const char* menu_item_name = elem["name"];
    u8g2.drawStr( 0, render_menu, menu_item_name);
    render_menu+=8;
  }

  u8g2.sendBuffer();
  delay(100);
}
