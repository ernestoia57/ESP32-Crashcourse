#include <Arduino.h>

/*
   ============================================
   1. Definición de pines
   ============================================
   Aquí asignamos un pin para cada LED del semáforo
   y un pin para el botón.
*/
const int LED_ROJO = 25;
const int LED_AMARILLO = 26;
const int LED_VERDE = 27;
const int BOTON = 14;

/*
   ============================================
   2. Variable de interrupción
   ============================================
   Esta variable será modificada dentro de la ISR
   (rutina de interrupción), por eso debe ser volatile.

   volatile le dice al compilador que esta variable
   puede cambiar "de repente" fuera del flujo normal
   del programa.
*/
volatile bool reiniciarSemaforo = false;

/*
   ============================================
   3. Rutina de interrupción (ISR)
   ============================================
   Esta función se ejecuta automáticamente cuando
   ocurre el evento del botón.

   IRAM_ATTR se usa en ESP32 para indicar que la ISR
   debe colocarse en memoria rápida apropiada.
*/
void IRAM_ATTR botonPresionado() {
  reiniciarSemaforo = true;
}

/*
   ============================================
   4. Función para apagar todos los LEDs
   ============================================
   Es útil para evitar repetir código.
*/
void apagarLeds() {
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_AMARILLO, LOW);
  digitalWrite(LED_VERDE, LOW);
}

/*
   ============================================
   5. Función para encender solo un LED
   ============================================
   Recibe el pin del LED que queremos encender.
*/
void encenderSolo(int led) {
  apagarLeds();
  digitalWrite(led, HIGH);
}

/*
   ============================================
   6. Función de espera interrumpible
   ============================================
   En vez de usar delay(tiempo), esperamos poco a poco
   para poder revisar si el botón fue presionado.

   Si durante la espera se activa la interrupción,
   salimos inmediatamente.
*/
bool esperarConRevision(int tiempo_ms) {
  int paso = 10;  // revisamos cada 10 ms

  for (int t = 0; t < tiempo_ms; t += paso) {
    if (reiniciarSemaforo) {
      return true;  // indicar que se pidió reinicio
    }
    delay(paso);
  }

  return false; // no hubo reinicio durante la espera
}

void setup() {
  /*
     ============================================
     7. Configuración de pines
     ============================================
  */
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  /*
     INPUT_PULLUP:
     el botón estará normalmente en HIGH y al presionarlo
     pasará a LOW si está conectado a GND.
  */
  pinMode(BOTON, INPUT_PULLUP);

  /*
     ============================================
     8. Asociar la interrupción al botón
     ============================================
     attachInterrupt(pin, funcionISR, modo)

     FALLING significa que la interrupción ocurre cuando
     la señal pasa de HIGH a LOW, típico con INPUT_PULLUP
     al presionar un botón conectado a tierra.
  */
  attachInterrupt(digitalPinToInterrupt(BOTON), botonPresionado, FALLING);

  apagarLeds();
}

void loop() {
  /*
     ============================================
     9. Inicio de secuencia del semáforo
     ============================================
     Si se presionó el botón en cualquier momento,
     la variable reiniciarSemaforo será true.
     Entonces se reinicia la secuencia desde rojo.
  */

  // Antes de empezar una vuelta normal, limpiamos el pedido
  reiniciarSemaforo = false;

  /*
     ROJO
  */
  encenderSolo(LED_ROJO);
  if (esperarConRevision(3000)) {
    return; // volver al inicio del loop y empezar desde rojo
  }

  /*
     VERDE
  */
  encenderSolo(LED_VERDE);
  if (esperarConRevision(3000)) {
    return;
  }

  /*
     AMARILLO
  */
  encenderSolo(LED_AMARILLO);
  if (esperarConRevision(1000)) {
    return;
  }
}