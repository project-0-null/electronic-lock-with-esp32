#include <Arduino.h>
#include "teclado.h"
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

char teclas[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int linhas[] = {linha1, linha2, linha3, linha4};
int colunas[] = {coluna1, coluna2, coluna3, coluna4};

unsigned long ultimoTempo[4][4] = {0};
bool ultimoEstado[4][4] = {false};

//=================================================
//interrupçao de tempo
//=================================================
hw_timer_t * timer = NULL;
bool flag;

volatile char teclaPressionada = '\0';
volatile bool teclaNova = false;

volatile unsigned long ultimo_interrupçao = 0;
#define DEBOUNCE_TIME 50

//----->função de interrupção do timer<-----
void IRAM_ATTR onTimer(){
    flag = true;
    //timerAlarm
}
void setup(){
timer=timerBegin(0,80,true);
timerAttachInterrupt(timer, &onTimer, true);
}//???????????????????????????????????????? tem que verificar essa merda




//==================================================
//FUNÇÕES DO TECLADO
//==================================================

bool DetectaTecla(int linhaIDX, int colunaIDX){
    bool estado_atual = digitalRead(linhas[linhaIDX]);
    unsigned long now = millis(); //pega o tempo atual
    bool pressionada = false;

    if (now - ultimoTempo[linhaIDX][colunaIDX] >= DEBOUNCE_MS){//verifica se a diferença de tempo é maior ou igual ao tempo determinado de debouce, talvez mudar pra interrupção depois.
        if (estado_atual && !ultimoEstado[linhaIDX][colunaIDX]){//verifica se foi pressionado e nao se esta sendo segurado
            pressionada = true;
        }
        ultimoEstado[linhaIDX][colunaIDX] = estado_atual;
        ultimoTempo[linhaIDX][colunaIDX] = now;
    }
}
