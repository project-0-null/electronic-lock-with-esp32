#include <Arduino.h>
#include "pwm.h"

static int ultimaLeitura = -1;    // Última leitura do LDR

hw_timer_t *temp = NULL;           // Ponteiro para o temporizador hardware
volatile int ultimoDuty = -1;       // Último duty cycle aplicado (volatile pois é acessado na ISR)
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // Mutex para proteção de seção
//inclui a função de leitura do pwm

int leituraLDR() {
  return analogRead(POT_INPUT_PIN);
}

//configura o pwm
void setupPWM() {
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configura o canal primeiro
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);       // Anexa o pino ao canal depois
  // Inicia com brilho médio
  int dutyInicial = (DUTY_MIN + DUTY_MAX) / 2;
  ledcWrite(PWM_CHANNEL, dutyInicial);
  ultimoDuty = dutyInicial;

  Serial.println("PWM configurado com sucesso");
}
// Atualiza o brilho do LCD com monitoramento eficiente
void atualizaBrilhoLCD() {
  int leitura = leituraLDR();
  
  // Só processa se houve mudança significativa (threshold)
  if (abs(leitura - ultimaLeitura) < ADC_THRESHOLD) {
    return; // Sai da função sem fazer nada - economia de processamento!
  }
  
  ultimaLeitura = leitura;
  
  // IMPORTANTE: Invertido porque K é o catodo do LCD
  // Duty alto = menos brilho | Duty baixo = mais brilho
  // Mapeia com limites para garantir brilho mínimo
  int duty = map(leitura, 0, 4095, DUTY_MIN, DUTY_MAX);
  
  // Garante que duty fica dentro dos limites (segurança extra)
  duty = constrain(duty, DUTY_MIN, DUTY_MAX);
  
  // Histerese adicional: só atualiza se mudança > 30
  if (abs(duty - ultimoDuty) > 30) {
    ledcWrite(PWM_CHANNEL, duty);
    ultimoDuty = duty;
    
    // Calcula brilho percentual para exibição (invertido)
    int brilhoPercent = map(duty, DUTY_MAX, DUTY_MIN, BRILHO_MINIMO_PERCENT, BRILHO_MAXIMO_PERCENT);
    
    // Calcula tensões para debug
    float VIO = (duty / (float)MAX_DUTY_CYCLE) * Vk_max;
    float Vk = VIO / Gain + Vbe;
    
  }
}

