#include <Arduino.h>
#include "pwm.h"

//inclui a função de leitura do pwm
int leituraLDR() {
  int leitura = analogRead(POT_INPUT_PIN);
  return leitura;
}

//configura o pwm
void setupPWM() {
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configura o canal primeiro
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);       // Anexa o pino ao canal depois
}

void saidaPWM() {
  int duty = map(leituraLDR(), 0, 4095, 0, MAX_DUTY_CYCLE);
  static int uDuty = -1; 
  float VIO = (duty / (float)MAX_DUTY_CYCLE) * Vk_max; // Calcula a tensão de saída do PWM
  float Vk = VIO / Gain + Vbe; // Calcula a tensão no LDR usando o ganho e a tensão de base do transistor
  // Serial.print("VIO: ");
  // Serial.print(VIO);
  // Serial.print(" Vk: ");
  // Serial.println(Vk);
  
  if (abs(duty - uDuty) > 64) { // Histerese aumentada para S3, ou seja, só atualiza o PWM se a mudança for significativa
    ledcWrite(PWM_CHANNEL, duty);
    uDuty = duty;
  //   Serial.print("Brilho: "); 
  //   Serial.println(duty);
  // }
  }
}