#include <Arduino.h>

// Cambia esto si tu LED está en otro pin
static const int LED_PIN = 2;

// ESP32 PWM (LEDC)
static const int PWM_CHANNEL   = 0;      // canal 0..15
static const int PWM_FREQ_HZ   = 5000;   // frecuencia PWM
static const int PWM_RES_BITS  = 8;      // 8 bits => duty 0..255

void setup() {
  Serial.begin(115200);
  delay(200);

  // -------------------------
  // 1) LED ON/OFF (digital)
  // -------------------------
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Parte 1: LED ON/OFF");
  for (int i = 0; i < 6; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }

  // -------------------------
  // 2) PWM con LEDC (ESP32)
  // -------------------------
  Serial.println("Parte 2: PWM (fade)");

  // Configura PWM en el pin
  ledcSetup(PWM_CHANNEL, PWM_FREQ_HZ, PWM_RES_BITS);
  ledcAttachPin(LED_PIN, PWM_CHANNEL);
  
  //ledcAttach(LED_PIN, PWM_FREQ_HZ, PWM_RES_BITS)
  //ledcwrite(LED_PIN, duty)
  
  // -----------------------------
  // 3) Control con serial (ESP32)
  // -----------------------------
  Serial.println("Parte 3: control serial");
  
}

void loop() {
  // Fade in
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(PWM_CHANNEL, duty);
    delay(10);
  }

  // Fade out
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(PWM_CHANNEL, duty);
    delay(10);
  }
}