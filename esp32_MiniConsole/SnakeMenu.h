#ifndef SNAKEMENU_H
#define SNAKEMENU_H
#include<U8g2lib.h>
class SnakeMenu{
  private:
    bool buttonState;
    bool gameOverButtonState;
  public:
    SnakeMenu();
    void drawMainMenu(U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI& u8g2);
    void drawHelpMenu(U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI& u8g2);
    void drawGameOverPanel(U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI& u8g2, int score);
    void changeButtonState();
    void changeGameOverButtonState();
    bool getButtonState();
    bool getGameOverButtonState();
};
#endif