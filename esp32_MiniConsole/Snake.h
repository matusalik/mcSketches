#ifndef SNAKE_H
#define SNAKE_H
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
    void nextFrameSnake();
    void setDirection(int direction);
    void addNewSegment();
};
#endif