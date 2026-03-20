#include <Arduino.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Botón
const int BUTTON_PIN = 18;

// FreeRTOS: cola para eventos de botón
QueueHandle_t buttonQueue;

// Estado
volatile unsigned long lastIsrMs = 0;

// ISR: solo manda un evento a la cola
void IRAM_ATTR isrButton() {
  unsigned long now = millis();
  if (now - lastIsrMs < 200) return;  // debounce simple
  lastIsrMs = now;

  int event = 1;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(buttonQueue, &event, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

// Task que maneja LCD (trabajo “pesado” fuera de la ISR)
void lcdTask(void* pvParameters) {
  int pressCount = 0;
  int mode = 0;

  lcd.init();
  lcd.backlight();

  // Pantalla inicial
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FreeRTOS + LCD");
  lcd.setCursor(0, 1);
  lcd.print("Btn para cambiar");

  while (true) {
    int event;
    // Espera bloqueada a evento (no consume CPU)
    if (xQueueReceive(buttonQueue, &event, portMAX_DELAY) == pdTRUE) {
      pressCount++;
      mode = (mode + 1) % 3;

      lcd.clear();
      if (mode == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Modo 0: Intro");
        lcd.setCursor(0, 1);
        lcd.print("Pulsaciones:");
        lcd.print(pressCount);
      } else if (mode == 1) {
        lcd.setCursor(0, 0);
        lcd.print("Modo 1: Conteo");
        lcd.setCursor(0, 1);
        lcd.print("N=");
        lcd.print(pressCount);
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Modo 2: Tiempo");
        lcd.setCursor(0, 1);
        lcd.print(millis() / 1000);
        lcd.print(" s");
      }

      Serial.print("Evento ISR. Conteo=");
      Serial.print(pressCount);
      Serial.print(" modo=");
      Serial.println(mode);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Crear cola (hasta 10 eventos)
  buttonQueue = xQueueCreate(10, sizeof(int));
  if (buttonQueue == NULL) {
    Serial.println("Error: no se pudo crear la cola");
    while (true) delay(1000);
  }

  // Crear task LCD (stack y prioridad moderada)
  xTaskCreatePinnedToCore(
    lcdTask,        // función
    "lcdTask",      // nombre
    4096,           // stack
    NULL,           // params
    1,              // prioridad
    NULL,           // handle
    1               // core (0 o 1)
  );

  // Interrupción del botón
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isrButton, FALLING);

  Serial.println("Listo: presiona el boton");
}

void loop() {
  // Nada aquí: FreeRTOS task se encarga
  vTaskDelay(pdMS_TO_TICKS(1000));
}