//aqui a gente vai fazer basicamente uma maquina de estados pra fazer o menu e etc
//obrigado eliete 🙏
#include <Arduino.h>
#include <teclado.h>
#include <lcd.h>
#define tranca 47
// ====================================================== 
// funções da maquina de estados
// ======================================================
char senha_entry[5]; //array pra armazenar a senha digitada, 4 caracteres + null terminator
char senha_1[5] = "1234";
char senha_2[5] = "5678";
char senha_3[5] = "4321";
char senha_ADM[5] = "0000";
char* senhas[] = {senha_1, senha_2, senha_3};


int verifica_senha(char* senha_entry){
    if(strcmp(senha_entry, senha_ADM) == 0){
        return 4; //senha de administrador
    }
    for (int i = 0; i < 3; i++) {
        if (strcmp(senha_entry, senhas[i]) == 0) {
            return i + 1; // Retorna o número da senha (1, 2 ou 3)
        }
    }
    return 0;
}

// ======================================================
// função setup e loop
// ======================================================
int contador_leituras = 0;
void setup() {
    Serial.begin(115200);
    delay(1000); 
    inicializa_teclado();
    Serial.println("Teclado inicializado");
    initializationLCD();
    Serial.println("LCD inicializado");

    lcdSetCursor(0,0);
    lcd_print("Senha:");
    pinMode(tranca,OUTPUT);
    digitalWrite(tranca,LOW);
}

int coluna=0;
void loop() {
    // teclado_varredura();

    char tecla = ler_teclado();
    
    if (tecla != '\0') {
       Serial.print(tecla);
       lcdSetCursor(coluna,1);
       lcdWrite('*');
       senha_entry[coluna] = tecla;
       coluna++;
       if(coluna >= NUM_COLUNAS || tecla == '*'){ //se a coluna chegar no limite ou se a tecla for '*', verifica a senha
           coluna=0;
           if (verifica_senha(senha_entry) == 1 || verifica_senha(senha_entry) == 2 || verifica_senha(senha_entry) == 3){
            digitalWrite(tranca,HIGH);
           }
           switch (verifica_senha(senha_entry)) {
               case 1:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Miguel");
                    break;
                case 2:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Lorenzo");
                    break;
                case 3:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Tais");
                    break;
                case 4:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Administrador");
                    delay(5000);//5seg
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("Menu:");
                    lcdSetCursor(0,1);
                    lcd_print("1-senha 2-Tranca");
                    //meter uma funçao void e resumir isso aqui 
                    break;
                
                case 0:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("senha invalida");
                    break;
           }
           
        }
    }
}    