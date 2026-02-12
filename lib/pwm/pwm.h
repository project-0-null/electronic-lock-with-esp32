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

// ============================================
// FUNÇÕES DO PWM
// ============================================

int leituraLDR();
void setupPWM();
void saidaPWM();