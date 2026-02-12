#include <Arduino.h>
#include <teclado.h>
#include <lcd.h>

#define tranca 47
// ====================================================== 
// variaveis gloabis 
// ======================================================
extern char senha_1[5];
extern char senha_2[5];
extern char senha_3[5];
extern char senha_ADM[5];
extern char* senhas[];

extern unsigned long tempo_tranca = 5000; //tempo que a tranca fica aberta em milissegundos


void adm_menu(){

    lcdSetCursor(0,0);
    lcd_print("1-senhas");
    lcdSetCursor(0,1);
    lcd_print("2-tempo de tranca");
    lcd_print("3-sair");
    char tecla = ler_teclado();




}