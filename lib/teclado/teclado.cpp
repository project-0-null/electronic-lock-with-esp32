#include "teclado.h"

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
const int colunas[] = {coluna1, coluna2, coluna3};


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

// void teclado_varredura(){
//     // if(tecla_disponivel){
//     //     return; //se ja tem uma tecla disponivel, nao faz a varredura
//     // }
//     //essa função aqui energiza as colunas e depois desenergiza, e ai usa a função detecta tecla pra verificar se foi pressionada(se teve saida na linha)
//     for (int col=0; col < NUM_COLUNAS;col++){
//         digitalWrite(colunas[col],HIGH);
//         // delayMicroseconds(50);
//         for (int lin=0; lin<NUM_LINHAS;lin++){
//             if( DetectaTecla(lin,col)){
//                 ultimaTeclaPressionada = teclas[lin][col];
//                 tecla_disponivel = true;
//                 //digitalWrite(colunas[col],LOW); //desenergiza a coluna apos detectar a tecla
//                 return; //sai da função para evitar varredura desnecessária
//             }
//             // delayMicroseconds(50);
//         }
//         digitalWrite(colunas[col],LOW); //desenergiza a coluna apos a varredura
//     }
// }

// bool DetectaTecla(int linhaIDX, int colunaIDX){
//     //essa aqui faz debounce e detecta pressionamento
//     bool estado_atual = digitalRead(linhas[linhaIDX]);
//     unsigned long now = millis(); //pega o tempo atual
//     bool pressionada = false;

//     if (now - ultimoTempo[linhaIDX][colunaIDX] >= DEBOUNCE_MS){//verifica se a diferença de tempo é maior ou igual ao tempo determinado de debouce, talvez mudar pra interrupção depois.
//         if (estado_atual && !ultimoEstado[linhaIDX][colunaIDX]){//verifica se foi pressionado e nao se esta sendo segurado
//             pressionada = true;
//         }
//         ultimoEstado[linhaIDX][colunaIDX] = estado_atual;
//         ultimoTempo[linhaIDX][colunaIDX] = now;
//     }
//     return pressionada;
// }

void IRAM_ATTR onTimer(){
    // teclado_varredura();

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
