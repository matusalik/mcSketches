#include "SnakeMenu.h"
SnakeMenu::SnakeMenu(){
  this->buttonState = true;
}

void SnakeMenu::drawMainMenu(U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI& u8g2){
  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  if (buttonState) {
    u8g2.drawButtonUTF8(10, 50, U8G2_BTN_INV | U8G2_BTN_BW2, 0, 2, 2, "PLAY");
    u8g2.drawButtonUTF8(75, 50, U8G2_BTN_BW2, 0, 2, 2, "HELP");
  } else if (!buttonState) {
    u8g2.drawButtonUTF8(10, 50, U8G2_BTN_BW2, 0, 2, 2, "PLAY");
    u8g2.drawButtonUTF8(75, 50, U8G2_BTN_INV | U8G2_BTN_BW2, 0, 2, 2, "HELP");
  }
  u8g2.drawStr(37, 15, "SNAKE");
  u8g2.setBitmapMode(true);
  //u8g2.drawXBMP(3, 2, 128, 32, snakeBM);
  u8g2.sendBuffer();
  delay(200);
}

void SnakeMenu::changeButtonState(){
  this->buttonState = !this->buttonState;
}