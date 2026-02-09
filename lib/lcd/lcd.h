#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

//==================================================
// configuração GPIO do LCD 
//==================================================
#define RS 3
#define E 8
#define D4 18
#define D5 17
#define D6 16
#define D7 15


#define LCD_CLEAR_DISPLAY 0x01

// ============================================
// FUNÇÕES DO LCD
// ============================================

void EnablePulse();
void write4bits(uint8_t value);
void Sendbyte(uint8_t value, bool isData);
void lcdWrite(char c);
void lcdCommand(uint8_t cmd);
void lcdSetCursor(uint8_t col, uint8_t line);
void LCDclear();
void initializationLCD();
void lcd_print(const char* str);

#endif