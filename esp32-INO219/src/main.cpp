#include <Arduino.h>          // Librería base de Arduino: setup(), loop(), Serial, etc.
#include <Wire.h>             // Librería para comunicación I2C
#include <Adafruit_INA219.h>  // Librería del sensor INA219 de Adafruit

/* =========================================================
   1. CREACIÓN DEL OBJETO DEL SENSOR
   =========================================================
   Aquí creamos una instancia del sensor INA219.

   La librería Adafruit permite crear el objeto con la dirección
   I2C por defecto (0x40), que es la más común en este módulo.

   Si tu INA219 tuviera otra dirección, podría declararse como:
   Adafruit_INA219 ina219(0x41);
   pero normalmente 0x40 está bien.
   ========================================================= */
Adafruit_INA219 ina219;

/* =========================================================
   2. FUNCIÓN setup()
   =========================================================
   setup() se ejecuta una sola vez al iniciar el ESP32.

   Aquí normalmente:
   - iniciamos el puerto serial
   - iniciamos I2C
   - inicializamos el sensor
   - verificamos que el sensor responda correctamente
   ========================================================= */
void setup() {
    /* -----------------------------------------------------
       Iniciar comunicación serial
       -----------------------------------------------------
       Esto permite ver mensajes en el monitor serial de
       PlatformIO o del entorno Arduino.
       115200 es una velocidad muy común y estable.
       ----------------------------------------------------- */
    Serial.begin(115200);

    /* -----------------------------------------------------
       Pequeña pausa inicial
       -----------------------------------------------------
       A veces conviene esperar un poco para que el monitor
       serial alcance a conectarse y no perder los primeros
       mensajes.
       ----------------------------------------------------- */
    delay(1000);

    Serial.println("========================================");
    Serial.println("Inicializando ESP32 + INA219...");
    Serial.println("========================================");

    /* -----------------------------------------------------
       Iniciar el bus I2C
       -----------------------------------------------------
       En ESP32 no siempre se usan automáticamente los pines
       por defecto, así que los indicamos manualmente:

       SDA = GPIO 21
       SCL = GPIO 22

       Esta es la configuración más común en ESP32-WROOM-32.
       ----------------------------------------------------- */
    Wire.begin(21, 22);

    /* -----------------------------------------------------
       Inicializar el sensor INA219
       -----------------------------------------------------
       ina219.begin() intenta comunicarse con el dispositivo
       por I2C usando la dirección configurada.

       Si devuelve false, significa que:
       - no encontró el sensor
       - la dirección I2C puede ser incorrecta
       - hay problema de cableado
       - falta alimentación
       ----------------------------------------------------- */
    if (!ina219.begin()) {
        Serial.println("ERROR: no se encontró el sensor INA219.");
        Serial.println("Revisa:");
        Serial.println("- Alimentación del modulo");
        Serial.println("- Conexiones SDA y SCL");
        Serial.println("- GND común");
        Serial.println("- Dirección I2C del sensor");
        
        /* -------------------------------------------------
           Bucle infinito de error
           -------------------------------------------------
           Si el sensor no fue detectado, detenemos la
           ejecución normal y dejamos el equipo aquí.
           ------------------------------------------------- */
        while (1) {
            delay(10);
        }
    }

    Serial.println("Sensor INA219 detectado correctamente.");

    /* -----------------------------------------------------
       Configuración de calibración
       -----------------------------------------------------
       La librería Adafruit ofrece varios modos de calibración
       ya preparados.

       setCalibration_32V_2A():
       - rango de voltaje de bus hasta 32 V
       - corriente típica hasta 2 A
       - útil para muchos módulos comerciales INA219

       Esta suele ser una buena configuración general.

       Otras opciones comunes en algunas versiones de la
       librería incluyen:
       - setCalibration_32V_1A()
       - setCalibration_16V_400mA()

       Si tu carga consume poco, una calibración más ajustada
       puede dar mejor resolución.
       ----------------------------------------------------- */
    ina219.setCalibration_32V_2A();

    Serial.println("Calibración configurada: 32V / 2A");
    Serial.println("Sistema listo para medir.");
    Serial.println();
}

/* =========================================================
   3. FUNCIÓN loop()
   =========================================================
   loop() se ejecuta de forma repetitiva e indefinida mientras
   el ESP32 esté encendido.

   Aquí leeremos:
   - voltaje del shunt
   - voltaje del bus
   - corriente
   - potencia
   - voltaje estimado en la carga
   ========================================================= */
void loop() {
    /* -----------------------------------------------------
       Variables para almacenar las mediciones
       -----------------------------------------------------
       Cada función de la librería devuelve directamente el
       valor ya convertido a unidades físicas.
       ----------------------------------------------------- */
    float shuntVoltage_mV = 0.0;  // Voltaje en el shunt, en milivoltios
    float busVoltage_V    = 0.0;  // Voltaje del bus, en voltios
    float current_mA      = 0.0;  // Corriente, en miliamperios
    float power_mW        = 0.0;  // Potencia, en miliwatts
    float loadVoltage_V   = 0.0;  // Voltaje estimado en la carga, en voltios

    /* -----------------------------------------------------
       Lectura del voltaje en el shunt
       -----------------------------------------------------
       El INA219 mide la caída de tensión sobre su resistencia
       shunt interna/externa del módulo.

       Esta caída suele ser pequeña y se expresa en mV.

       Este valor es clave porque a partir de él el sensor
       calcula la corriente.
       ----------------------------------------------------- */
    shuntVoltage_mV = ina219.getShuntVoltage_mV();

    /* -----------------------------------------------------
       Lectura del voltaje del bus
       -----------------------------------------------------
       Este es el voltaje principal del lado medido.
       Por ejemplo, si mides una batería o una fuente,
       aquí obtendrás ese valor aproximado.
       ----------------------------------------------------- */
    busVoltage_V = ina219.getBusVoltage_V();

    /* -----------------------------------------------------
       Lectura de la corriente
       -----------------------------------------------------
       Esta función devuelve directamente la corriente en mA,
       usando la calibración configurada previamente.
       ----------------------------------------------------- */
    current_mA = ina219.getCurrent_mA();

    /* -----------------------------------------------------
       Lectura de la potencia
       -----------------------------------------------------
       Devuelve la potencia en mW calculada por el INA219.
       ----------------------------------------------------- */
    power_mW = ina219.getPower_mW();

    /* -----------------------------------------------------
       Cálculo del voltaje en la carga
       -----------------------------------------------------
       En muchos ejemplos de Adafruit se estima el voltaje real
       sobre la carga sumando:
       
       loadVoltage = busVoltage + shuntVoltage

       Pero ojo:
       - busVoltage está en voltios
       - shuntVoltage está en milivoltios

       Entonces primero convertimos mV a V dividiendo entre 1000.
       ----------------------------------------------------- */
    loadVoltage_V = busVoltage_V + (shuntVoltage_mV / 1000.0);

    /* -----------------------------------------------------
       Impresión de resultados en monitor serial
       -----------------------------------------------------
       Aquí mostramos todas las magnitudes de manera ordenada.
       ----------------------------------------------------- */
    Serial.println("========================================");
    Serial.println("Lecturas del INA219");
    Serial.println("========================================");

    Serial.print("Voltaje en shunt: ");
    Serial.print(shuntVoltage_mV, 3);  // 3 decimales
    Serial.println(" mV");

    Serial.print("Voltaje de bus  : ");
    Serial.print(busVoltage_V, 3);
    Serial.println(" V");

    Serial.print("Corriente       : ");
    Serial.print(current_mA, 3);
    Serial.println(" mA");

    Serial.print("Potencia        : ");
    Serial.print(power_mW, 3);
    Serial.println(" mW");

    Serial.print("Voltaje en carga: ");
    Serial.print(loadVoltage_V, 3);
    Serial.println(" V");

    /* -----------------------------------------------------
       Interpretación rápida opcional
       -----------------------------------------------------
       Este bloque ayuda a entender el estado general.
       No es obligatorio, pero es útil para depurar.
       ----------------------------------------------------- */
    if (current_mA < 1.0) {
        Serial.println("Observación: corriente muy baja o carga casi desconectada.");
    } else {
        Serial.println("Observación: medición normal de corriente.");
    }

    Serial.println();

    /* -----------------------------------------------------
       Espera antes de la siguiente medición
       -----------------------------------------------------
       delay(1000) = 1000 ms = 1 segundo
       Así evitamos saturar el monitor serial y obtenemos
       lecturas periódicas fáciles de leer.
       ----------------------------------------------------- */
    delay(1000);
}