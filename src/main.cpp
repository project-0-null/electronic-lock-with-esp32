#include <Arduino.h>

// ============================================
// CONFIGURAÇÃO DO TECLADO
// ============================================
#define LINHA1 48
#define LINHA2 45
#define LINHA3 0  
#define LINHA4 35

#define COLUNA1 36
#define COLUNA2 37
#define COLUNA3 38
#define COLUNA4 39

int linhas[] = {LINHA1, LINHA2, LINHA3, LINHA4};
int colunas[] = {COLUNA1, COLUNA2, COLUNA3, COLUNA4};

// Matriz de teclas
char teclas[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Variáveis para debounce
unsigned long ultimoTempo[4][4] = {0};
bool ultimoEstado[4][4] = {false};
#define DEBOUNCE_MS 50

// ============================================
// CONFIGURAÇÃO DO LCD
// ============================================
#define RS 13
#define E  14
#define D4 7
#define D5 6
#define D6 5
#define D7 4

// ============================================
// FUNÇÕES DO LCD
// ============================================

// Pulso no Enable
void pulseEnable() {
  digitalWrite(E, HIGH);
  delayMicroseconds(1);
  digitalWrite(E, LOW);
  delayMicroseconds(50);
}

// Envia 4 bits para o LCD
void write4bits(uint8_t value) {
  digitalWrite(D4, (value >> 0) & 0x01);
  digitalWrite(D5, (value >> 1) & 0x01);
  digitalWrite(D6, (value >> 2) & 0x01);
  digitalWrite(D7, (value >> 3) & 0x01);
  pulseEnable();
}

// Envia um byte completo (em 2 partes)
void sendByte(uint8_t value, bool isData) {
  // RS = HIGH para dados, LOW para comando
  digitalWrite(RS, isData ? HIGH : LOW);
  
  // Envia primeiro os 4 bits altos
  write4bits(value >> 4);
  
  // Envia depois os 4 bits baixos
  write4bits(value & 0x0F);
}

// Funções de alto nível para o LCD
void lcdCommand(uint8_t cmd) {
  sendByte(cmd, false);
}

void lcdWrite(char c) {
  sendByte(c, true);
}

void lcdPrint(const char* str) {
  while (*str) {
    lcdWrite(*str++);
  }
}

void lcdSetCursor(uint8_t col, uint8_t row) {
  uint8_t address;
  if (row == 0) {
    address = 0x80 + col;  // Linha 1 começa em 0x80
  } else {
    address = 0xC0 + col;  // Linha 2 começa em 0xC0
  }
  lcdCommand(address);
}

void lcdClear() {
  lcdCommand(0x01);
  delay(2);  // Este comando demora 1.6ms
}

// Inicialização do LCD
void lcdBegin() {
  // Configura pinos como saída
  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  
  // Aguarda LCD ligar
  delay(50);
  
  // Sequência de inicialização do modo 4-bit
  write4bits(0x03);
  delayMicroseconds(4500);
  
  write4bits(0x03);
  delayMicroseconds(150);
  
  write4bits(0x03);
  delayMicroseconds(150);
  
  write4bits(0x02);
  delayMicroseconds(150);
  
  // Configura display: 2 linhas, fonte 5x8
  lcdCommand(0x28);
  delayMicroseconds(50);
  
  // Liga display, sem cursor
  lcdCommand(0x0C);
  delayMicroseconds(50);
  
  // Limpa display
  lcdClear();
  
  // Modo de entrada
  lcdCommand(0x06);
  delayMicroseconds(50);
}

// ============================================
// FUNÇÕES DO TECLADO
// ============================================

// Detecta borda de subida com debounce
bool detectaTecla(int linhaIdx, int colunaIdx) {
  bool estadoAtual = digitalRead(colunas[colunaIdx]);
  unsigned long agora = millis();
  bool pressionada = false;
  
  if (agora - ultimoTempo[linhaIdx][colunaIdx] >= DEBOUNCE_MS) {
    if (estadoAtual && !ultimoEstado[linhaIdx][colunaIdx]) {
      pressionada = true;
    }
    ultimoEstado[linhaIdx][colunaIdx] = estadoAtual;
    ultimoTempo[linhaIdx][colunaIdx] = agora;
  }
  
  return pressionada;
}

// Varre o teclado e retorna tecla pressionada
char lerTeclado() {
  for (int i = 0; i < 4; i++) {
    // Desativa todas as linhas
    for (int x = 0; x < 4; x++) {
      digitalWrite(linhas[x], LOW);
    }
    
    // Ativa apenas a linha atual
    digitalWrite(linhas[i], HIGH);
    delayMicroseconds(10);  // Pequena estabilização
    
    // Verifica colunas desta linha
    for (int j = 0; j < 4; j++) {
      if (detectaTecla(i, j)) {
        // Aguarda soltar a tecla (anti-repique)
        while(digitalRead(colunas[j]) == HIGH) {
          delay(1);
        }
        return teclas[i][j];
      }
    }
    
    // Desativa a linha
    digitalWrite(linhas[i], LOW);
  }
  
  return '\0';  // Nenhuma tecla pressionada
}

// ============================================
// SETUP E LOOP PRINCIPAIS
// ============================================
void setup() {
  Serial.begin(9600);
  Serial.println("Sistema Teclado + LCD");
  
  // Configura teclado
  for (int i = 0; i < 4; i++) {
    pinMode(linhas[i], OUTPUT);
    digitalWrite(linhas[i], LOW);
  }
  
  for (int i = 0; i < 4; i++) {
    pinMode(colunas[i], INPUT_PULLDOWN);
  }
  
  // Configura LCD
  lcdBegin();
  
  // Mensagem inicial
  lcdSetCursor(0, 0);
  lcdPrint("Teclado Teste");
  lcdSetCursor(0, 1);
  lcdPrint("Pressione:");
  
  // delay(2000);
  // lcdClear();
  // lcdPrint("insira a senha:");
}

void loop() {
  static int posicao = 0;
  char tecla = lerTeclado();
  
  if (tecla != '\0') {
    Serial.print("Tecla: ");
    Serial.println(tecla);
    
    // Mostra no LCD
    lcdSetCursor(posicao % 16, posicao / 16);
    lcdWrite(tecla);
    
    posicao++;
    if (posicao >= 32) {  // 16x2 = 32 caracteres
      posicao = 0;
      lcdClear();
    }
    
    // Pequeno delay para não ler muito rápido
    //delay(100);
  }
}