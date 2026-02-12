#include <Arduino.h>

// ============================================
// CONFIGURAÇÃO DO PWM
// ============================================
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION  12 
#define DELAY_MS  1000
#define LED_OUTPUT_PIN 4
#define POT_INPUT_PIN 9
const int MAX_DUTY_CYCLE = (int)(pow(2,PWM_RESOLUTION)-1);

#define Vk_max 3.3
#define Vk_min 0.0
#define Gain 180
#define Vbe 0.7

#define TIMER_INTERVAL_US 50000  // 50ms = 50000 microsegundos (20Hz de atualização)
extern hw_timer_t *temp;        // Timer hardware para interrupção

// ============================================
// LIMITES DE BRILHO
// ============================================
#define BRILHO_MINIMO_PERCENT 15  // LCD nunca fica com menos de 15% de brilho
#define BRILHO_MAXIMO_PERCENT 100 // Brilho máximo

// Calcula os limites de duty cycle (invertido porque K é catodo)
const int DUTY_MIN = (int)(MAX_DUTY_CYCLE * (1.0 - BRILHO_MAXIMO_PERCENT / 100.0)); // Duty baixo = brilho alto
const int DUTY_MAX = (int)(MAX_DUTY_CYCLE * (1.0 - BRILHO_MINIMO_PERCENT / 100.0)); // Duty alto = brilho baixo

// ============================================
// CONFIGURAÇÃO DA INTERRUPÇÃO POR THRESHOLD
// ============================================
#define ADC_THRESHOLD 50         // Threshold de mudança para disparar interrupção (ajuste conforme necessário)
extern volatile bool atualizarPWM; // Flag para sinalizar quando atualizar PWM


// ============================================
// FUNÇÕES DO PWM
// ============================================


void atualizaBrilhoLCD();
int leituraLDR();
void setupPWM();


