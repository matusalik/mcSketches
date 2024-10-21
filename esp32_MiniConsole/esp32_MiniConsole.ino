#include<U8g2lib.h>
#define LeftButton 26
#define RightButton 25
#define AcceptButton 33
#define DATA 23
#define DC 19
#define CLOCK 18
#define CS 5
#define RESET 4
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, CLOCK, DATA, CS, DC, RESET);
void setup() {
  Serial.begin(115200);
  pinMode(LeftButton, INPUT);
  pinMode(RightButton, INPUT);
  pinMode(AcceptButton, INPUT);
  u8g2.begin();
  u8g2.setContrast(60);
}

void loop() {
  u8g2.clearBuffer();
  u8g2.drawCircle(20, 20, 15);
  u8g2.sendBuffer();
  delay(100);
}