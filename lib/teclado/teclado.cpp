#include "teclado.h"

//controle para verificação para o debounce
volatile unsigned long ultimoTempo[NUM_LINHAS][NUM_COLUNAS] = {0};
volatile bool ultimoEstado[NUM_LINHAS][NUM_COLUNAS] = {false};

//controle do teclado
volatile char ultimaTeclaPressionada = '\0';
volatile bool tecla_disponivel = false;

//ponteiro para a interrupção
static hw_timer_t * timer = NULL;

//variaveis do teclado
const int linhas[] = {linha1, linha2, linha3, linha4};
const int colunas[] = {coluna1, coluna2, coluna3,coluna4};


const char teclas[NUM_LINHAS][NUM_COLUNAS] = {//coloquei assim pra poder verificar tanto no woki tanto quando na montagem do lab
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};




//==================================================
//FUNÇÕES DO TECLADO
//==================================================

void inicializa_teclado(){
    //energiza as colunas
    for (int i =0;i<NUM_COLUNAS;i++){
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

void teclado_varredura(){
    // if(tecla_disponivel){
    //     return; //se ja tem uma tecla disponivel, nao faz a varredura
    // }
    //essa função aqui energiza as colunas e depois desenergiza, e ai usa a função detecta tecla pra verificar se foi pressionada(se teve saida na linha)
    for (int col=0; col < NUM_COLUNAS;col++){
        digitalWrite(colunas[col],HIGH);
        // delayMicroseconds(50);
        for (int lin=0; lin<NUM_LINHAS;lin++){
            if( DetectaTecla(lin,col)){
                ultimaTeclaPressionada = teclas[lin][col];
                tecla_disponivel = true;
                //digitalWrite(colunas[col],LOW); //desenergiza a coluna apos detectar a tecla
                return; //sai da função para evitar varredura desnecessária
            }
            // delayMicroseconds(50);
        }
        digitalWrite(colunas[col],LOW); //desenergiza a coluna apos a varredura
    }
}

bool DetectaTecla(int linhaIDX, int colunaIDX){
    //essa aqui faz debounce e detecta pressionamento
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
    return pressionada;
}

void IRAM_ATTR onTimer(){
    teclado_varredura();
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
