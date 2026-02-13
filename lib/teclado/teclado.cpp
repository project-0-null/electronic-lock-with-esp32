#include "teclado.h"
//==========================================================
//variaveis
//==========================================================


//controle para verificação para o debounce
volatile unsigned long ultimoTempo[NUM_LINHAS][NUM_COLUNAS] = {0};
volatile bool ultimoEstado[NUM_LINHAS][NUM_COLUNAS] = {false};

//controle do teclado
volatile char ultimaTeclaPressionada = '\0';
volatile bool tecla_disponivel = false;
volatile bool teclado_liberado = true;

//ponteiro para a interrupção
static hw_timer_t * timer = NULL;

//variaveis do teclado
const int linhas[] = {linha1, linha2, linha3, linha4};
const int colunas[] = { coluna1, coluna2, coluna3};


const char teclas[NUM_LINHAS][NUM_COLUNAS] = {//coloquei assim pra poder verificar tanto no woki tanto quando na montagem do lab
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

volatile int contadoresDebounce[NUM_LINHAS][NUM_COLUNAS] = {0};
const int LIMITE_DEBOUNCE = 3; //3 ciclos de 10ms ,ajustar esse valor conforme necessário para estabilidade


//==================================================
//FUNÇÕES DO TECLADO
//==================================================

void inicializa_teclado(){
    //energiza as colunas
    for (int i = 0;i<NUM_COLUNAS;i++){
        pinMode(colunas[i], OUTPUT);
        digitalWrite(colunas[i], LOW); //inicialmente desenergizadas
    }
    //configura as lnhas como entrada
    for(int i=0;i<NUM_LINHAS;i++){
        pinMode(linhas[i],INPUT_PULLDOWN);
    }

    //inicializa e/ou limpa os arrays de controle 
    for (int i =0; i<NUM_LINHAS; i++){
        for(int j =0;j<NUM_COLUNAS;j++){
            ultimoTempo[i][j]=0;
            ultimoEstado[i][j]=false;
        }
    }
    //configura a varredura (10ms = 100hz)
    //alocaçao 0, 80(1Mhz), crescente
    timer= timerBegin(0,80,true);
    //interrupção
    timerAttachInterrupt(timer, &onTimer, true);
    //define o periodo -> 10000us = 10ms
    timerAlarmWrite(timer,10000,true);//aparentemente esse true significa auto-reload que eu acho que é ficar se armando sozinho
    //adivinha oq esse aqui faz
    timerAlarmEnable(timer);
}


void IRAM_ATTR onTimer(){//coloquei aqui porque é a função de varredura, tem que ser rápida e sem bloqueios

    bool alguma_tecla_detectada_no_ciclo = false;
    char tecla_candidata = '\0';

    for (int col=0; col < NUM_COLUNAS;col++){
        digitalWrite(colunas[col],HIGH);//energiza a coluna
        ets_delay_us(20); //pequeno delay para estabilizar o sinal, ajustar conforme necessário

        for(int lin=0; lin<NUM_LINHAS;lin++){
            //verifica se HIGH (perssionada)
            if(digitalRead(linhas[lin])){
                contadoresDebounce[lin][col]++;

                //se atingiu o limite de estabilidade e nao havia tecla pressionada antes, considera como nova tecla
                if(contadoresDebounce[lin][col] >= LIMITE_DEBOUNCE )
                {
                    tecla_candidata= teclas[lin][col];
                    
                    alguma_tecla_detectada_no_ciclo = true;
                }
                
            }else{
                contadoresDebounce[lin][col] = 0;//reseta o contador se a tecla nao estiver pressionada
            }
        }
        digitalWrite(colunas[col],LOW);
        if (alguma_tecla_detectada_no_ciclo) {
            break; 
        }
        
    }
    // 2. Lógica de decisão (fora dos loops de varredura)
    if (alguma_tecla_detectada_no_ciclo) {
        // Só registra se o teclado estava liberado antes (Borda de subida)
        // E se a aplicação já consumiu a tecla anterior (!tecla_disponivel)
        if (teclado_liberado && !tecla_disponivel) {
            ultimaTeclaPressionada = tecla_candidata;
            tecla_disponivel = true;
            teclado_liberado = false; // Bloqueia novas leituras (trava)
        }
    } else {
        // Se varreu tudo e ninguém está apertando nada, libera a trava
        teclado_liberado = true; 
    }
}

char ler_teclado(){
    if(tecla_disponivel){
        char tecla = ultimaTeclaPressionada;
        //limpa apos leitura
        ultimaTeclaPressionada= '\0';
        tecla_disponivel=false;
        return tecla;
    }
    return '\0';//nenhuma tecla disponivel, nao retorna nada uai :|
}
