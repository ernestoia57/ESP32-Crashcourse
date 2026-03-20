#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =========================
// CONFIGURACIÓN DE PINES
// =========================
const uint8_t PIN_BOTON = 18;   // Botón de interrupción
const uint8_t PIN_POT   = 35;   // ADC del potenciómetro

// =========================
// LCD I2C
// Dirección 0x27 según tu JSON
// Tamaño: 16 columnas x 2 filas
// =========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// =========================
// VARIABLES GLOBALES
// =========================
volatile bool interrupcionBoton = false;   // Bandera activada por la ISR

uint8_t modoActual = 0;                    // 0, 1, 2
unsigned long tiempoInicio = 0;            // Tiempo desde que inicia el sistema
unsigned long ultimoRefrescoLCD = 0;       // Control de refresco de pantalla
unsigned long ultimoCambioValido = 0;      // Antirrebote por software en loop

unsigned long contadorModo0 = 0;           // Contador mostrado en modo 0
unsigned long ultimoIncrementoContador = 0;

// =========================
// ISR DEL BOTÓN
// Debe ser corta y simple:
// solo marca una bandera.
// =========================
void IRAM_ATTR isrBoton() {
  interrupcionBoton = true;
}

// =========================
// FUNCIÓN PARA CAMBIAR DE MODO
// =========================
void cambiarModo() {
  modoActual++;
  if (modoActual > 2) {
    modoActual = 0;
  }
}

// =========================
// FUNCIÓN PARA MOSTRAR MODO 0
// "Mensaje inicial + contador"
// =========================
void mostrarModo0() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modo 0: Inicio");

  lcd.setCursor(0, 1);
  lcd.print("Contador: ");
  lcd.print(contadorModo0);
}

// =========================
// FUNCIÓN PARA MOSTRAR MODO 1
// Tiempo activo en segundos
// =========================
void mostrarModo1() {
  unsigned long segundosActivos = (millis() - tiempoInicio) / 1000UL;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modo 1: Tiempo");

  lcd.setCursor(0, 1);
  lcd.print(segundosActivos);
  lcd.print(" s activos");
}

// =========================
// FUNCIÓN PARA MOSTRAR MODO 2
// Valor del potenciómetro
// ADC ESP32: 0 a 4095
// =========================
void mostrarModo2() {
  int valorADC = analogRead(PIN_POT);
  float voltaje = (valorADC * 3.3f) / 4095.0f;
  int porcentaje = map(valorADC, 0, 4095, 0, 100);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pot: ");
  lcd.print(valorADC);

  lcd.setCursor(0, 1);
  lcd.print(voltaje, 2);
  lcd.print("V ");
  lcd.print(porcentaje);
  lcd.print("%");
}

// =========================
// SETUP
// =========================
void setup() {
  Serial.begin(115200);

  // Configuración del botón:
  // Como el botón va de GPIO18 a GND,
  // usamos resistencia pull-up interna.
  pinMode(PIN_BOTON, INPUT_PULLUP);

  // Configuración del ADC
  pinMode(PIN_POT, INPUT);
  analogReadResolution(12); // ESP32: 12 bits -> 0 a 4095

  // Inicialización I2C y LCD
  Wire.begin(21, 22); // SDA = 21, SCL = 22
  lcd.init();
  lcd.backlight();

  // Mensaje de arranque
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  lcd.setCursor(0, 1);
  lcd.print("ESP32 + LCD");
  delay(1500);

  // Registrar tiempo de inicio
  tiempoInicio = millis();
  ultimoIncrementoContador = millis();

  // Interrupción por flanco de bajada:
  // al pulsar, el pin pasa de HIGH a LOW
  attachInterrupt(digitalPinToInterrupt(PIN_BOTON), isrBoton, FALLING);

  // Mostrar primer modo
  mostrarModo0();
}

// =========================
// LOOP PRINCIPAL
// =========================
void loop() {
  unsigned long ahora = millis();

  // ---------------------------------
  // 1. PROCESAR EVENTO DE INTERRUPCIÓN
  // ---------------------------------
  if (interrupcionBoton) {
    // Limpiamos bandera lo antes posible
    interrupcionBoton = false;

    // Antirrebote básico por software
    if (ahora - ultimoCambioValido > 200) {
      ultimoCambioValido = ahora;
      cambiarModo();

      Serial.print("Cambio de modo -> ");
      Serial.println(modoActual);

      // Refrescar inmediatamente la pantalla
      switch (modoActual) {
        case 0:
          mostrarModo0();
          break;
        case 1:
          mostrarModo1();
          break;
        case 2:
          mostrarModo2();
          break;
      }
    }
  }

  // ---------------------------------
  // 2. ACTUALIZAR CONTADOR DEL MODO 0
  //    Cada 1 segundo
  // ---------------------------------
  if (ahora - ultimoIncrementoContador >= 1000) {
    ultimoIncrementoContador = ahora;
    contadorModo0++;
  }

  // ---------------------------------
  // 3. REFRESCO PERIÓDICO DEL LCD
  //    Evitamos actualizar demasiado rápido
  // ---------------------------------
  if (ahora - ultimoRefrescoLCD >= 400) {
    ultimoRefrescoLCD = ahora;

    switch (modoActual) {
      case 0:
        mostrarModo0();
        break;

      case 1:
        mostrarModo1();
        break;

      case 2:
        mostrarModo2();
        break;
    }
  }
}