#ifndef TECLADO_H  // Proteção para não incluir o arquivo duas vezes
#define TECLADO_H

#include <Arduino.h>

#define DEBOUNCE_MS 100
#define NUM_LINHAS 4
#define NUM_COLUNAS 3

//==================================================
// configuração do teclado matricial 4x4
//==================================================
#define linha1 35
#define linha2 37
#define linha3 36
#define linha4 38

#define coluna1 41
#define coluna2 40
#define coluna3 39


//=================================================
// declaração de funções
//=================================================


bool DetectaTecla(int linhaIDX, int colunaIDX);
void IRAM_ATTR onTimer();
void teclado_varredura();
void inicializa_teclado();
char ler_teclado();

#endif
