#include "teclado.h"

//controle para verificação para o debounce


//==================================================
//FUNÇÕES DO TECLADO
//==================================================

void teclado_init(){
    //energiza as colunas
    for (int i =0;i<NUM_COLUNAS;i++){
        pinMode(colunas[i], LOW);
    }
    //configura as lnhas como entrada
    for(int i=0;i<NUM_LINHAS;i++){
        pinMode(linhas[i],INPUT);
    }

    //inicializa e/ou limpa os arrays de controle 
    for (int i =0; i<NUM_LINHAS; i++){
        for(int j =0;j<NUM_COLUNAS;j++){
            ultimoTempo[i][i]=0;
            ultimoEstado[i][i]=false;
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
    //essa função aqui energiza as colunas e depois desenergiza, e ai usa a função detecta tecla pra verificar se foi pressionada(se teve saida na linha)
    for (int col=0; col < NUM_COLUNAS;col++){
        digitalWrite(colunas[col],HIGH);
        // delayMicroseconds(10);
        for (int lin=0; lin<NUM_LINHAS;lin++){
            if( DetectaTecla(lin,col)){
                ultimaTeclaPressionada = teclas[lin][col];
                tecla_disponivel = true;
            }
        }
        digitalWrite(colunas[col],LOW);
    }
}

static bool DetectaTecla(int linhaIDX, int colunaIDX){
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

static void IRAM_ATTR onTimer(){
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
