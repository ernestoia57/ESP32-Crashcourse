# Proyecto: Monitoreo de voltaje, corriente y potencia con INA219 y ESP32-WROOM-32

Este proyecto usa un **ESP32-WROOM-32** y un **sensor INA219** para medir:

- Voltaje del bus (conexión)
- Corriente de la carga
- Potencia consumida
- Voltaje estimado en la carga

El sensor se comunica con el ESP32 mediante **I2C**, por lo que la integración es simple y muy útil para monitoreo de fuentes DC, baterías, módulos y pequeñas cargas electrónicas.

Para mayor información acerca de la librería y los procesos de comunicación entre el **DevModule** y el **INA219**, consultar **https://github.com/adafruit/Adafruit_INA219**.

El INA219 es un sensor de **corriente** bidireccional. Estas mediciones, en la configuración correcta nos permiten hacer cálculos como el de la tensión, y potencia en la carga conectada, a pesar de no medir dichos datos directamente.

---

## 1. Componentes recomendados

- 1 × ESP32-WROOM-32
- 1 × Módulo INA219
- Jumpers
- Fuente DC (para alimentar la carga a medir)
- Carga DC (LED, resistencia, motor pequeño, etc.)

---

## 2. Conexiones

### Conexión entre ESP32 e INA219

| INA219 | ESP32-WROOM-32 |
|--------|-----------------|
| VCC    | 3.3V            |
| GND    | GND             |
| SDA    | GPIO 21         |
| SCL    | GPIO 22         |

### Conexión de potencia para medir la carga

Asegurarse de que el INA219 se encuentra **en serie con el positivo de la carga**.


| Conexión | Descripción |
|----------|-------------|
| Fuente (+) | VIN+ del INA219 |
| VIN- del INA219 | Positivo de la carga |
| Fuente (-) | Negativo de la carga |

### Esquema general

```text
ESP32                  INA219
-----                  ------
3.3V   --------------> VCC
GND    --------------> GND
GPIO21 --------------> SDA
GPIO22 --------------> SCL

Fuente (+) ----------> VIN+
VIN- ----------------> (+) Carga
Fuente (-) ----------> (-) Carga
