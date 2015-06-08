/**
 * @file    fonttest.c
 * @brief   font api test for u8g lib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-04-19
 * @copyright GPL
 */
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <U8glib.h>

#include "fontutf8u8g.h"


#if 0
#define OLED_MOSI   9 // SDA
#define OLED_CLK   10 // SCL
#define OLED_DC    11 // D/C (data or command)
#define OLED_CS    12 // no such pin
#define OLED_RESET 13 // RST
#else
#define OLED_MOSI   4 // SDA
#define OLED_CLK    5 // SCL
#define OLED_DC     2 // D/C (data or command)
#define OLED_CS    12 // no such pin
#define OLED_RESET  3 // RST
#endif

// SW SPI Com: SCK = 10, MOSI = 9, CS = 12, A0 = 11, reset=13
#define U8GVAL_SCK   OLED_CLK
#define U8GVAL_MOSI  OLED_MOSI
#define U8GVAL_CS    OLED_CS
#define U8GVAL_A0    OLED_DC
#define U8GVAL_RESET OLED_RESET

//U8GLIB_SH1106_128X64 u8g(U8GVAL_SCK, U8GVAL_MOSI, U8GVAL_CS, U8GVAL_A0, U8GVAL_RESET);

//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
// 或者
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);

void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void setup(void) {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void u8g_ascii_1() {
  char s[2] = " ";
  uint8_t x, y;
  u8g.drawStr( 0, 0, "ASCII page 1");
  for( y = 0; y < 6; y++ ) {
    for( x = 0; x < 16; x++ ) {
      s[0] = y*16 + x + 32;
      u8g.drawStr(x*7, y*10+10, s);
    }
  }
}


void u8g_chinese() {
  char s[] = _U8GT("可");
  u8g.drawStr( 0, 0, "Chinese Glyph");
  utf8_draw (&u8g, 10, 30, s);
}

uint8_t draw_state = 0;

void draw0(void) {
  u8g_prepare();
  switch(draw_state >> 3) {
    case 6: u8g_ascii_1(); break;
    case 9: u8g_chinese(); break;
  }
}

void draw(void) {
  u8g_prepare();
  u8g_chinese();
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // increase the state
  draw_state++;
  if ( draw_state > 9*8 )
    draw_state = 0;
  
  // rebuild the picture after some delay
  delay(500);

}


