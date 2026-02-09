#ifndef TECLADO_H  // Proteção para não incluir o arquivo duas vezes
#define TECLADO_H

#include <Arduino.h>

#define DEBOUNCE_MS 100
#define NUM_LINHAS 4
#define NUM_COLUNAS 4

//==================================================
// configuração do teclado matricial 4x4
//==================================================
#define linha1 48
#define linha2 45
#define linha3 0 
#define linha4 35

#define coluna1 36
#define coluna2 37
#define coluna3 38
#define coluna4 39

//=================================================
// declaração de funções
//=================================================


bool DetectaTecla(int linhaIDX, int colunaIDX);
void IRAM_ATTR onTimer();
void teclado_varredura();
void inicializa_teclado();
char ler_teclado();

#endif
