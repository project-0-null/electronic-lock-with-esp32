//aqui a gente vai fazer basicamente uma maquina de estados pra fazer o menu e etc
//obrigado eliete 🙏
#include <Arduino.h>
#include <teclado.h>
#include <lcd.h>
#include <pwm.h>
#include <adm.h>
// #include <adm>
#define tranca 47
// ====================================================== 
// funções da maquina de estados
// ======================================================
char senha_entry[5]; //array pra armazenar a senha digitada, 4 caracteres + null terminator
char senha_1[5] = "1234"; //senha do miguel
char senha_2[5] = "4321"; //senha da lorenzo
char senha_3[5] = "5678"; //senha da tais
char senha_ADM[5] = "0000"; //senha do administrador
char* senhas[5] = {senha_1, senha_2, senha_3, senha_ADM}; //array de ponteiros (:o) para as senhas

unsigned long tempo_tranca = 5000; //tempo que a tranca fica aberta em milissegundos

void t_tranca();

int verifica_senha(char* senha_entry){
    for (int i = 0; i < 3; i++) {
        if (strcmp(senha_entry, senhas[i]) == 0) {
            return i + 1; // Retorna o número da senha (1, 2 ou 3)
        }
        
    }
    return 0; //senha invalida    
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

    setupPWM();
    Serial.println("PWM inicializado");
}
bool tranca_aberta = false;
int coluna=0;
void loop() {
    // teclado_varredura();
    saidaPWM();
    
    char tecla = ler_teclado();
    
    if (tecla != '\0') {
       Serial.print(tecla);//pra eu ver
    //    Serial.print(senha_entry);
       lcdSetCursor(coluna,1);
       lcdWrite('*');
       senha_entry[coluna] = tecla;
       coluna++;
       if(coluna >= NUM_COLUNAS || tecla == '*'){ //se a coluna chegar no limite ou se a tecla for '*', verifica a senha
           coluna=0;
        
           switch (verifica_senha(senha_entry)) {
               case 1:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Miguel");
                    t_tranca();
                    memset(senha_entry, '\0', sizeof(senha_entry));
                    break;
                case 2:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Lorenzo");
                    t_tranca();
                    memset(senha_entry, '\0', sizeof(senha_entry));
                    break;
                case 3:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Tais");
                    t_tranca();
                    memset(senha_entry, '\0', sizeof(senha_entry));
                    break;
                case 4:
                    memset(senha_entry, '\0', sizeof(senha_entry));
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("bem-vindo");
                    lcdSetCursor(0,1);
                    lcd_print("Administrador");
                    delay(5000);//5seg
                    LCDclear();
                    //... 
                    break;
                
                case 0:
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("senha invalida");
                    memset(senha_entry, '\0', sizeof(senha_entry));
                    delay(3000);//1seg
                    LCDclear();
                    lcdSetCursor(0,0);
                    lcd_print("Senha:");
                    break;
           }
           
        }else if (tecla == '#'){
            LCDclear();
            lcd_print("Senha:");
            for(int i=0;i<5;i++){
            senha_entry[i]='\0';
            };//zera a senha
        }
        
    }
}    

void t_tranca(){
    tranca_aberta = true;
    digitalWrite(tranca,HIGH);
    delay(tempo_tranca);
    digitalWrite(tranca,LOW);
}

