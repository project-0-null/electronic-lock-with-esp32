#ifndef ADM_H
#define ADM_H

#include <Arduino.h>
#include <lcd.h>
#include <teclado.h>

#define NUM_USUARIOS 3
#define TAMANHO_SENHA 5
#define tranca 42

// senhas dos usuários e do admin (acessíveis pelo main.cpp)
extern char senha_1[TAMANHO_SENHA];
extern char senha_2[TAMANHO_SENHA];
extern char senha_3[TAMANHO_SENHA];
extern char senha_ADM[TAMANHO_SENHA];
extern unsigned long tempo_tranca;
extern char* senhas[NUM_USUARIOS + 1]; // +1 para a senha do administrador


// funções do menu admin
void menu_adm();

#endif