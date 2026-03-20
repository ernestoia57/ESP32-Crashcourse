#include <Arduino.h>

// ======================================================
// CONFIGURACIÓN DE PINES
// ======================================================

// Entradas ADC para los potenciómetros
const int POT_R_PIN = 34;   // Solo entrada, ideal para ADC
const int POT_G_PIN = 35;   // Solo entrada
const int POT_B_PIN = 32;   // ADC

// Salidas PWM para el LED RGB
const int LED_R_PIN = 25;
const int LED_G_PIN = 26;
const int LED_B_PIN = 27;

// ======================================================
// CONFIGURACIÓN PWM (LEDC en ESP32)
// ======================================================
const int PWM_FREQ = 5000;      // Frecuencia en Hz
const int PWM_RESOLUTION = 8;   // 8 bits: valores de 0 a 255

const int PWM_CHANNEL_R = 0;
const int PWM_CHANNEL_G = 1;
const int PWM_CHANNEL_B = 2;

// ======================================================
// CONFIGURACIÓN ADC
// ======================================================
// ESP32 suele devolver valores entre 0 y 4095 con resolución de 12 bits
const int ADC_MAX = 4095;
const int PWM_MAX = 255;

// ======================================================
// FUNCIÓN DE MAPEO ADC -> PWM
// ======================================================
int adcToPwm(int adcValue) {
  // Limitar el valor de adcValue dentro del rango permitido
  adcValue = constrain(adcValue, 0, ADC_MAX);

  // Conversión proporcional directa
  return map(adcValue, 0, ADC_MAX, 0, PWM_MAX);
}

// ======================================================
// SETUP
// ======================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== Control RGB con potenciometros ===");

  // Configuración ADC
  analogReadResolution(12); // 0 a 4095

  // Configuración PWM
  ledcSetup(PWM_CHANNEL_R, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_G, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(LED_R_PIN, PWM_CHANNEL_R);
  ledcAttachPin(LED_G_PIN, PWM_CHANNEL_G);
  ledcAttachPin(LED_B_PIN, PWM_CHANNEL_B);

  // Inicialmente apagado
  ledcWrite(PWM_CHANNEL_R, 0);
  ledcWrite(PWM_CHANNEL_G, 0);
  ledcWrite(PWM_CHANNEL_B, 0);
}

// ======================================================
// LOOP PRINCIPAL
// ======================================================
void loop() {
  // Leer los tres potenciómetros
  int adcR = analogRead(POT_R_PIN);
  int adcG = analogRead(POT_G_PIN);
  int adcB = analogRead(POT_B_PIN);

  // Convertir ADC a PWM
  int pwmR = adcToPwm(adcR);
  int pwmG = adcToPwm(adcG);
  int pwmB = adcToPwm(adcB);

  // Escribir PWM al LED RGB
  ledcWrite(PWM_CHANNEL_R, pwmR);
  ledcWrite(PWM_CHANNEL_G, pwmG);
  ledcWrite(PWM_CHANNEL_B, pwmB);

  // Mostrar información en monitor serial
  Serial.print("ADC -> R: ");
  Serial.print(adcR);
  Serial.print(" G: ");
  Serial.print(adcG);
  Serial.print(" B: ");
  Serial.print(adcB);

  Serial.print(" | PWM -> R: ");
  Serial.print(pwmR);
  Serial.print(" G: ");
  Serial.print(pwmG);
  Serial.print(" B: ");
  Serial.println(pwmB);

  delay(100);
}