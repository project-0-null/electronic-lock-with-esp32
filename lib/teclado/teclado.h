#include <Arduino.h>

#define DEBOUNCE_MS 50
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

const int linhas[] = {linha1, linha2, linha3, linha4};
const int colunas[] = {coluna1, coluna2, coluna3, coluna4};


const char teclas[NUM_LINHAS][NUM_COLUNAS] = {//coloquei assim pra poder verificar tanto no woki tanto quando na montagem do lab
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

volatile unsigned long ultimoTempo[NUM_LINHAS][NUM_COLUNAS] = {0};
volatile bool ultimoEstado[NUM_LINHAS][NUM_COLUNAS] = {false};

static hw_timer_t * timer = NULL;

//controle do teclado
volatile char ultimaTeclaPressionada = '\0';
volatile bool tecla_disponivel = false;

volatile unsigned long ultimo_interrupçao = 0;


//=================================================
// declaração de funções
//=================================================


static bool DetectaTecla(int linhaIDX, int colunaIDX);
static void IRAM_ATTR onTimer();
void teclado_varredura();
void inicializa_teclado(){}
char ler_teclado();
