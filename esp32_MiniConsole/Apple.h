#ifndef APPLE_H
#define APPLE_H
#include<U8g2lib.h>
#include<Arduino.h>
class Apple{
  private:
    int x;
    int y;
    bool appleExists;
  public:
    Apple();
    void drawApple(U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI& u8g2);
    void eatApple();
    int getX();
    int getY();

};
#endif