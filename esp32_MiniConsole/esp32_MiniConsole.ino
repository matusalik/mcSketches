#include<U8g2lib.h>
#include<string.h>
#include"Snake.h"
#include"SnakeMenu.h"
#include"States.h"
#include"Apple.h"

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
SnakeMenu snakeMenu;
States state = MAIN_MENU;
bool isRightPressed = false;
bool isLeftPressed = false;
bool drawPlus10 = false;
int score = 0;
int scoreCounter = 0;
int drawPlus10Counter = 0;
int tempAppleCoords[2];
Apple apple;

//INTERRUPTS
void IRAM_ATTR LeftButtonISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    if(state == MAIN_MENU){
      snakeMenu.changeButtonState();
    }
    else if(state == GAME_PANEL){
      isLeftPressed = true;
    }
    else if(state == GAME_OVER){
      snakeMenu.changeGameOverButtonState();
    }
    last_button_time = button_time;
  }
}

void IRAM_ATTR RightButtonISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    if(state == MAIN_MENU){
      snakeMenu.changeButtonState();
    }
    else if(state == GAME_PANEL){
      isRightPressed = true;
    }
    else if(state == GAME_OVER){
      snakeMenu.changeGameOverButtonState();
    }
    last_button_time = button_time;
  }
}

void IRAM_ATTR AcceptButtonISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    if(state == MAIN_MENU){
      if(snakeMenu.getButtonState()){
        state = GAME_PANEL;
      }
      else if(!snakeMenu.getButtonState()){
        state = HELP_PANEL;
      }
    }
    else if(state == HELP_PANEL){
      state = MAIN_MENU;
    }
    else if(state == GAME_OVER){
      if(snakeMenu.getGameOverButtonState()){
        score = 0;
        scoreCounter = 0;
        state = GAME_PANEL;
      }
      else if(!snakeMenu.getGameOverButtonState()){
        score = 0;
        scoreCounter = 0;
        state = MAIN_MENU;
      }
    }
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
  tempAppleCoords[0] = apple.getX();
  tempAppleCoords[1] = apple.getY();
  u8g2.begin();
  u8g2.setContrast(60);
}

void loop() {
  u8g2.clearBuffer();
  if(state == MAIN_MENU){
    snakeMenu.drawMainMenu(u8g2);
  }
  else if(state == HELP_PANEL){
    snakeMenu.drawHelpMenu(u8g2);
  }
  else if(state == GAME_PANEL){
    if(isRightPressed){
      snake.turnSnake(true);
      isRightPressed = false;
    }
    if(isLeftPressed){
      snake.turnSnake(false);
      isLeftPressed = false;
    }
    scoreCounter++;
    if(scoreCounter == 10){
      score++;
      scoreCounter = 0;
    }
    if(snake.checkForApples(apple)){
      drawPlus10 = true;
      score += 10;
    }
    if(drawPlus10){
      drawPlus10Counter++;
      if(drawPlus10Counter < 10){
        u8g2.setFont(u8g2_font_trixel_square_tr);
        u8g2.drawStr(tempAppleCoords[0]-2, tempAppleCoords[1]-4, "+10");
      }
      else{
        drawPlus10 = false;
        drawPlus10Counter = 0;
        tempAppleCoords[0] = apple.getX();
        tempAppleCoords[1] = apple.getY();
      }
    }
    drawGamePanel();
  }
  else if(state == GAME_OVER){
    snakeMenu.drawGameOverPanel(u8g2, score);
  }
  u8g2.sendBuffer();
  delay(100);
}

void drawGamePanel(){
  u8g2.drawFrame(0,0,128,64);
  u8g2.setFont(u8g2_font_trixel_square_tr);
  String temp = "SCORE: " + String(score);
  const char* c = temp.c_str();
  u8g2.drawStr(3, 8, c);
  snake.drawSnake(u8g2);
  snake.nextFrameSnake();
  apple.drawApple(u8g2);
  if(snake.checkForCollision()){
    state = GAME_OVER;
  }
}