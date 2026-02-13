#include <Arduino.h>
#include "lcd.h"


// ============================================
// FUNÇÕES DO LCD
// ============================================

void EnablePulse() {
    digitalWrite(E, LOW);
    delayMicroseconds(1);
    digitalWrite(E, HIGH);
    delayMicroseconds(1);//tempo de pulso
    digitalWrite(E, LOW);
    delayMicroseconds(100);//tempo para o comando ser processado
    //       __
    //______/  \______
    //low   high  low 
}

void write4bits(uint8_t value){
    //aqui ele arreda e compara com 0000 0001 para pegar o primeiro bit, so que ele arreda 4 vezes que é a fomra que o LCD lê
    digitalWrite(D4, (value >> 0) & 0x01);
    digitalWrite(D5, (value >> 1) & 0x01);
    digitalWrite(D6, (value >> 2) & 0x01);
    digitalWrite(D7, (value >> 3) & 0x01);
    EnablePulse();
}

void Sendbyte(uint8_t value, bool isData) {
    digitalWrite(RS, isData ? HIGH : LOW); //RS =  if (isData == true){ HIGH;} else {LOW;}
    write4bits(value >> 4); //envia os 4 ultimos bits, pq aq ele arreda pra direita 4 vezes
    write4bits(value & 0x0F); //envia os 4 primeiros bits comparando com 0000 1111, e excluindo os ultimos 4 bits

}

//caso eu queira escrever um caractere ou um comando 
void lcdWrite(char c) {Sendbyte(c, true);}
void lcdCommand(uint8_t cmd) {Sendbyte(cmd, false);}


void lcdSetCursor(uint8_t col, uint8_t line) {
    uint8_t adress;
    if (line == 0) {
        adress = 0x80 +col; //linha 1 começa em 0x80
    } else {
        adress = 0xC0 +col; //linha 2 começa em 0xC0
    }
    lcdCommand(adress);
}

void LCDclear(){
    lcdCommand(LCD_CLEAR_DISPLAY);//comando que limpa o display
    delayMicroseconds(2000); //esse comando demora mais pra ser executado
}

void lcd_print(const char* str){
    while(*str){
        lcdWrite(*str++);
    }
}


void initializationLCD(){
    pinMode(RS, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);

    delay(50);

  // Sequência de inicialização do modo 4-bit
  write4bits(0x03);
  delayMicroseconds(4500);
  
  write4bits(0x03);
  delayMicroseconds(150);
  
  write4bits(0x03);
  delayMicroseconds(150);
  
  write4bits(0x02);
  delayMicroseconds(150);

  // Configura display: 2 linhas, fonte 5x8
  lcdCommand(0x28);
  delayMicroseconds(50);
  
  // Liga display, sem cursor
  lcdCommand(0x0C);
  delayMicroseconds(50);
  
  // Limpa display
  LCDclear();
  
  // Modo de entrada
  lcdCommand(0x06);
  delayMicroseconds(50);
}


