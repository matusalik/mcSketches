#include <Arduino.h>
#include <U8g2lib.h>
#include<esp_random.h>
#define LBT 26  //LEFT BUTTON
#define RBT 25 //RIGHT BUTTON
#define ABT 33  //ACCEPT BUTTON
#define MAXSIZE 100 // MAX SIZE OF THE SNAKE
#define DATA 23
#define DC 19
#define CLOCK 18
#define CS 5
#define RESET 4
struct segment{
  int x;
  int y;
};
bool mainMenuButtonState = true;
int snakeSize = 7;
int direction = 0; //0 - east, 1 - south, 2 - west, 3 - north
segment body[MAXSIZE];
segment head;
int appleCoords[2];
bool appleExists = false;
enum States{
    MAIN_MENU,
    HELP_PANEL,
    GAME_PANEL
}state;
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, CLOCK, DATA, CS, DC, RESET);
//LCD PIN - ARDU PIN
//1 - 9
//2 - 6
//3 - 7
//4 - 8
//5 - 9
//6 - 5V
//7 - GND
//8 - 5V
//9 - GND
static const unsigned char snakeBM[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xf0, 0x3f, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x1f, 0x00,
  0x18, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x07, 0x38, 0x00, 0x0c, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc0, 0x81, 0x63, 0x00, 0x04, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xc0, 0xc3, 0x00,
  0x86, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x70, 0xc0, 0x83, 0x01, 0xc2, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf0, 0xc0, 0x83, 0x01, 0xc2, 0x3f, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x80, 0x80, 0x01,
  0xc2, 0x73, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xf8, 0x00, 0x80, 0x01, 0xc2, 0xc1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x80, 0xc0, 0x01, 0x82, 0xc1, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x80, 0xff, 0x00,
  0x82, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7c, 0x80, 0xff, 0x00, 0x82, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x66, 0x00, 0x02, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x70, 0x00,
  0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x1c, 0x00, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0f, 0x00, 0x84, 0x07, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x03, 0x00,
  0x8c, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x03, 0x78, 0x00, 0x00, 0x8c, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0x07, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0x03, 0x0e, 0x00, 0x00, 0x18, 0x8f, 0x3f, 0x00,
  0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x07, 0x00, 0x00,
  0x18, 0x80, 0xff, 0x83, 0xff, 0x1f, 0xfe, 0x01, 0xfc, 0xff, 0x01, 0xfc,
  0x03, 0x03, 0x00, 0x00, 0x30, 0x00, 0x3e, 0xff, 0xe7, 0x0f, 0xfe, 0xff,
  0xff, 0xbf, 0xcf, 0xff, 0x81, 0x01, 0x00, 0x00, 0x70, 0x00, 0x00, 0xfe,
  0xc0, 0x0f, 0x3c, 0xfc, 0xe1, 0x3f, 0xfc, 0xf9, 0x80, 0x00, 0x00, 0x00,
  0xe0, 0x00, 0x00, 0xfc, 0x80, 0x07, 0x00, 0xfc, 0xe1, 0x0f, 0xfe, 0x00,
  0x80, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
  0xc0, 0x07, 0x7c, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0xfc, 0x03,
  0x60, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x1f,
  0x00, 0x80, 0xbf, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff,
  0xff, 0x1f, 0x00, 0xf8, 0x7f, 0xf8, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const unsigned char snakeSmolBM[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x8e, 0x03, 0x0c, 0x06, 0x00, 0x00, 0x00, 0x00, 0x67, 0x06,
  0x06, 0x0c, 0x00, 0x00, 0x00, 0x80, 0x67, 0x04, 0xc2, 0x19, 0x00, 0x00,
  0x00, 0x80, 0x00, 0x04, 0x62, 0x3f, 0x00, 0x00, 0x00, 0x80, 0xe0, 0x07,
  0x32, 0x30, 0x00, 0x00, 0x00, 0x80, 0x01, 0x03, 0x1b, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xc7, 0x01, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00,
  0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x12, 0xf0, 0x7f, 0x00,
  0x00, 0x80, 0x41, 0x00, 0x72, 0x38, 0xef, 0x81, 0x7f, 0xe0, 0x40, 0x00,
  0xc6, 0x0f, 0x8f, 0xff, 0xde, 0x39, 0x60, 0x00, 0x84, 0x07, 0x00, 0x1e,
  0x1e, 0x0f, 0x20, 0x00, 0x8c, 0x07, 0x00, 0x1e, 0x00, 0x06, 0x30, 0x00,
  0x18, 0xe0, 0x03, 0xe0, 0x03, 0x80, 0x1f, 0x00, 0x70, 0x30, 0x06, 0x38,
  0x0e, 0xf8, 0x00, 0x00, 0xc0, 0x18, 0x0c, 0x0c, 0x38, 0x0e, 0x00, 0x00,
  0x80, 0x0f, 0xf8, 0x07, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
void drawMainMenu() {
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  if (mainMenuButtonState) {
    u8g2.drawButtonUTF8(10, 50, U8G2_BTN_INV | U8G2_BTN_BW2, 0, 2, 2, "PLAY");
    u8g2.drawButtonUTF8(75, 50, U8G2_BTN_BW2, 0, 2, 2, "HELP");
  } else if (!mainMenuButtonState) {
    u8g2.drawButtonUTF8(10, 50, U8G2_BTN_BW2, 0, 2, 2, "PLAY");
    u8g2.drawButtonUTF8(75, 50, U8G2_BTN_INV | U8G2_BTN_BW2, 0, 2, 2, "HELP");
  }
  u8g2.drawStr(37, 15, "SNAKE");
  u8g2.setBitmapMode(true);
  u8g2.drawXBMP(3, 2, 128, 32, snakeBM);
}
void drawHelpPanel() {
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawButtonUTF8(8, 53, U8G2_BTN_INV | U8G2_BTN_BW2, 0, 2, 2, "BACK");
  u8g2.setBitmapMode(true);
  u8g2.drawXBMP(0, 17, 60, 20, snakeSmolBM);
  u8g2.setFont(u8g2_font_littlemissloudonbold_tr);
  u8g2.drawStr(4, 15, "MANUAL");
  u8g2.drawRFrame(59, 2, 67, 60, 7);
  u8g2.setFont(u8g2_font_trixel_square_tr);
  u8g2.drawStr(65, 10, "That's a classic");
  u8g2.drawStr(67, 16, "game of SNAKE.");
  u8g2.drawStr(61, 22, "Controll the snake");
  u8g2.drawStr(63, 28, "with LEFT or RIGHT");
  u8g2.drawStr(65, 34, "buttons, hunt for");
  u8g2.drawStr(67, 40, "apples, live for");
  u8g2.drawStr(61, 47, "as long as you can");
  u8g2.drawStr(61, 53, "and don't bonk your");
  u8g2.drawStr(63, 59, "head on anything!");
  u8g2.setFont(u8g2_font_tenfatguys_tf);
}
void drawSnake(){
  for(int i = 0; i < snakeSize; i++){
    int x = body[i].x;
    int y = body[i].y;
    u8g2.drawFrame(x, y, 3, 3);
  }
}
void drawApple(){
  if(appleExists){
    u8g2.drawBox(appleCoords[0], appleCoords[1], 3,3);
  }
  else if(!appleExists){
    int x = random(2,127);
    int y = random(2, 63);
    appleCoords[0] = x;
    appleCoords[1] = y;
    appleExists = true;
  }
}
void prepareSnake(){
  for(int i = 0; i < snakeSize; i++){
    if(i != 0){
      body[i].x = head.x - 3*i;
      body[i].y = head.y;
    }  
    else{
      body[i].x = head.x;
      body[i].y = head.y;
    } 
  }
}
void nextFrameSnake(){
  for(int i = snakeSize; i >= 0; i--){
    if(i == 0){
      switch(direction){
        case 0:
          body[i].x = body[i].x + 3;
          break;
        case 1:
          body[i].y = body[i].y + 3;
          break;
        case 2:
          body[i].x = body[i].x - 3;
          break;
        case 3:
          body[i].y = body[i].y - 3;
          break;
      }
    }
    else{
      body[i].x = body[i-1].x;
      body[i].y = body[i-1].y;
    }
  }
}
void addNewSegment(){
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
void checkForApples(){
  if(((body[0].x >= appleCoords[0] && body[0].x <= appleCoords[0]+3) ||
     (body[0].x+3 <= appleCoords[0]+3 && body[0].x+3 >= appleCoords[0])) &&
     ((body[0].y >= appleCoords[1] && body[0].y <= appleCoords[1]+3)||
     (body[0].y+3 <= appleCoords[1]+3 && body[0].y+3 >= appleCoords[1]))){
      appleExists = false;
      addNewSegment();
     }
}
void drawGamePanel(){
  u8g2.drawFrame(0,0,128,64);
  drawSnake();
  checkForApples();
  drawApple();
  nextFrameSnake();
}
void setup() {
  Serial.begin(9600);
  pinMode(LBT, INPUT);
  pinMode(RBT, INPUT);
  pinMode(ABT, INPUT);
  randomSeed(analogRead(2));
  head.x = 64;
  head.y = 32;
  prepareSnake();
  u8g2.begin();
  u8g2.setContrast(60);
}

void loop() {
  if (state == MAIN_MENU) {
    if (digitalRead(LBT) == LOW) {
      Serial.println("LEFT button pressed");
      mainMenuButtonState = !mainMenuButtonState;
      delay(100);
    }
    if (digitalRead(RBT) == LOW) {
      Serial.println("RIGHT button pressed");
      mainMenuButtonState = !mainMenuButtonState;
      delay(100);
    }
    if (digitalRead(ABT) == LOW) {
      Serial.println("ACCEPT button pressed");
      if (mainMenuButtonState) {  //PLAY button selected
        state = GAME_PANEL;
      } 
      else if (!mainMenuButtonState) {  //HELP button selected
        state = HELP_PANEL;
      }
      delay(100);
    }
    u8g2.clearBuffer();
    drawMainMenu();
    u8g2.sendBuffer();
    delay(100);
  } 
  else if (state == HELP_PANEL) {
    if (digitalRead(ABT) == LOW) {
      state = MAIN_MENU;
      delay(100);
    }
    u8g2.clearBuffer();
    drawHelpPanel();
    u8g2.sendBuffer();
    delay(100);
  }
  else if(state == GAME_PANEL){
    if(digitalRead(RBT) == LOW && direction == 0){
      direction = 1;
      body[0].x = body[0].x - 3;  //MOVE HEAD from EAST to SOUTH
      body[0].y = body[0].y + 3;
      delay(100);
    }
    else if(digitalRead(RBT) == LOW && direction == 1){
      direction = 2;
      body[0].x = body[0].x - 3;  //MOVE HEAD from SOUTH to WEST
      body[0].y = body[0].y - 3;
      delay(100);
    }
    else if(digitalRead(RBT) == LOW && direction == 2){
      direction = 3;
      body[0].x = body[0].x + 3;  //MOVE HEAD from WEST to NORTH
      body[0].y = body[0].y - 3;
      delay(100);
    }
    else if(digitalRead(RBT) == LOW && direction == 3){
      direction = 0;
      body[0].x = body[0].x + 3;  //MOVE HEAD from NORTH to EAST
      body[0].y = body[0].y + 3;
      delay(100);
    }
    else if(digitalRead(LBT) == LOW && direction == 0){
      direction = 3;
      body[0].x = body[0].x - 3;  //MOVE HEAD from EAST to NORTH
      body[0].y = body[0].y - 3;
      delay(100);
    }
    else if(digitalRead(LBT) == LOW && direction == 3){
      direction = 2;
      body[0].x = body[0].x - 3;  //MOVE HEAD from NORTH to WEST
      body[0].y = body[0].y + 3;
      delay(100);
    }
    else if(digitalRead(LBT) == LOW && direction == 2){
      direction = 1;
      body[0].x = body[0].x + 3;  //MOVE HEAD from WEST to SOUTH
      body[0].y = body[0].y + 3;
      delay(100);
    }
    else if(digitalRead(LBT) == LOW && direction == 1){
      direction = 0;
      body[0].x = body[0].x + 3;  //MOVE HEAD from SOUTH to EAST
      body[0].y = body[0].y - 3;
      delay(100);
    }
    u8g2.clearBuffer();
    drawGamePanel();
    u8g2.sendBuffer();
    delay(200);
  }
}
