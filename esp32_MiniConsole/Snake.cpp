#include "Snake.h"
Snake::Snake() {
  this->snakeSize = 7;
  this->direction = 0;
  this->head.x = 64;
  this->head.y = 32;
  prepareSnake();
}

int Snake::getSnakeSize() {
  return this->snakeSize;
}

void Snake::incSnakeSize() {
  this->snakeSize++;
}

Segment Snake::getBody(int index){
  return this->body[index];
}

void Snake::prepareSnake() {
  for(int i = 0; i < snakeSize; i++){
    if(i != 0){
      this->body[i].x = this -> head.x - 3*i;
      this->body[i].y = this -> head.y;
    }  
    else{
      this->body[i].x = this -> head.x;
      this->body[i].y = this -> head.y;
    } 
  }
}

void Snake::nextFrameSnake(){
  for(int i = this->snakeSize; i >= 0; i--){
    if(i == 0){
      switch(this->direction){
        case 0:
          this->body[i].x = this->body[i].x + 3;
          break;
        case 1:
          this->body[i].y = this->body[i].y + 3;
          break;
        case 2:
          this->body[i].x = this->body[i].x - 3;
          break;
        case 3:
          this->body[i].y = this->body[i].y - 3;
          break;
      }
    }
    else{
      this->body[i].x = this->body[i-1].x;
      this->body[i].y = this->body[i-1].y;
    }
  }
}

void Snake::addNewSegment(){
  segment temp;
  if((body[snakeSize-1].x - body[snakeSize-2].x) < 0){
    temp.x = body[snakeSize-1].x - 3;
    temp.y = body[snakeSize-1].y;
    snakeSize++;
    body[snakeSize-1] = temp;
  }
  if((body[snakeSize-1].x - body[snakeSize-2].x) > 0){
    temp.x = body[snakeSize-1].x + 3;
    temp.y = body[snakeSize-1].y;
    snakeSize++;
    body[snakeSize-1] = temp;
  }
  if((body[snakeSize-1].y - body[snakeSize-2].y) < 0){
    temp.x = body[snakeSize-1].x;
    temp.y = body[snakeSize-1].y - 3;
    snakeSize++;
    body[snakeSize-1] = temp;
  }
  if((body[snakeSize-1].y - body[snakeSize-2].y) > 0){
    temp.x = body[snakeSize-1].x;
    temp.y = body[snakeSize-1].y + 3;
    snakeSize++;
    body[snakeSize-1] = temp;
  }
}