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

const int btn_pin_1 = 27;
const int btn_pin_2 = 20;

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void setup() {
  Serial.begin(9600);
  Wire.setSDA(22);
  Wire.setSCL(23);
  Wire.begin();  
  u8g2.begin();

  pinMode(btn_pin_1, INPUT);
  pinMode(btn_pin_2, INPUT);
}

int menu_item_index = 0;

void loop() {
  byte btn1_state = digitalRead(btn_pin_1);
  byte btn2_state = digitalRead(btn_pin_2);

  u8g2.clearBuffer();
  u8g2_prepare();

  int render_menu = 0;
  
  deserializeJson(doc, password);
  for (JsonObject elem : doc.as<JsonArray>()) {
    String menu_item_name = elem["name"];
    u8g2.drawStr( 0, render_menu, menu_item_name.c_str());
    render_menu+=8;
  }

  if(btn1_state == HIGH){
    menu_item_index += 1;
  }
  if(btn2_state == HIGH){
    menu_item_index -= 1;
  }

  u8g2.setCursor(0, render_menu);
  u8g2.print(menu_item_index);

  Serial.println(menu_item_index);
  
  u8g2.sendBuffer();
  delay(100);
}
