//aqui a gente vai fazer basicamente uma maquina de estados pra fazer o menu e etc
//obrigado eliete 🙏


#include <teclado.h>
#include <lcd.h>

int contador_leituras = 0;
void setup() {
    Serial.begin(115200);
    delay(1000); 
    inicializa_teclado();
    Serial.println("Teclado inicializado");

}

void loop() {
    //teclado_varredura();

    char tecla = ler_teclado();
    
    if (tecla != '\0') {
        contador_leituras++;
        
        Serial.println("================================");
        Serial.print("LEITURA #");
        Serial.println(contador_leituras);
        Serial.print("Tecla: ");
        Serial.println(tecla);
        Serial.print("Timestamp: ");
        Serial.print(millis());
        Serial.println("ms");
        Serial.println("================================");
        
        // Pequeno delay para visualizar no serial
        delay(10);
    }
}