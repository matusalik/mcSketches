#include"Apple.h"
Apple::Apple(){
  this->x = random(5, 123);
  this->y = random(5, 59);
  while((this->x < 20) && (this->y < 10)){
    this->x = random(5, 123);
    this->y = random(5, 59);
  }
  this->appleExists = true;
}

void Apple::drawApple(U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI& u8g2){
  if(this->appleExists){
    u8g2.drawBox(this->x, this->y, 3, 3);
  }
  else if(!this->appleExists){
    this->x = random(5, 123);
    this->y = random(5, 59);
    while((this->x < 20) && (this->y < 10)){
      this->x = random(5, 123);
      this->y = random(5, 59);
    }
    this->appleExists = true;
  }
}

void Apple::eatApple(){
  this->appleExists = false;
}

int Apple::getX(){
  return this->x;
}

int Apple::getY(){
  return this->y;
}