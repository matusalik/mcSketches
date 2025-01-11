#ifndef SNAKE_H
#define SNAKE_H
#include<U8g2lib.h>
#include "Segment.h"
class Snake{
  private:
    int snakeSize;
    int direction;
    Segment body[100];
    Segment head;
  public:
    Snake();
    int getSnakeSize();
    Segment getBody(int index);
    void incSnakeSize();
    void prepareSnake();
    void drawSnake(U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI& u8g2);
    void nextFrameSnake();
    void setDirection(int direction);
    void addNewSegment();
    void turnSnake(bool isRight);
    bool checkForCollision();
};
#endif