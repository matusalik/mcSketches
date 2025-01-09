#include<U8g2lib.h>
#include"Snake.h"

//PINS
#define LeftButton 33
#define RightButton 25
#define AcceptButton 26
#define DATA 23
#define DC 19
#define CLOCK 18
#define CS 5
#define RESET 4

//SCREEN
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, CLOCK, DATA, CS, DC, RESET);

//VARIABLES
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 
int x = 20;
Snake snake;

//INTERRUPTS
void IRAM_ATTR LeftButtonISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    last_button_time = button_time;
  }
}

void IRAM_ATTR RightButtonISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    last_button_time = button_time;
  }
}

void IRAM_ATTR AcceptButtonISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    last_button_time = button_time;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LeftButton, INPUT_PULLUP);
  pinMode(RightButton, INPUT_PULLUP);
  pinMode(AcceptButton, INPUT_PULLUP);
  attachInterrupt(LeftButton, LeftButtonISR, FALLING);
  attachInterrupt(RightButton, RightButtonISR, FALLING);
  attachInterrupt(AcceptButton, AcceptButtonISR, FALLING);
  u8g2.begin();
  u8g2.setContrast(60);
}

void loop() {
  u8g2.clearBuffer();
  drawSnake();
  snake.nextFrameSnake();
  u8g2.sendBuffer();
  delay(200);
}

void drawSnake(){
  for(int i = 0; i < snake.getSnakeSize(); i++){
    int x = snake.getBody(i).x;
    int y = snake.getBody(i).y;
    u8g2.drawFrame(x, y, 3, 3);
  }
}