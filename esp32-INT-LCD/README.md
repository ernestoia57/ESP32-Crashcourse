# Dashboard LCD con botón controlado por interrupciones

## Descripción del ejercicio

Este proyecto consiste en desarrollar un pequeño dashboard embebido con un **ESP32**, una **pantalla LCD 16x2 con interfaz I2C**, un **botón** y un **potenciómetro**.

El objetivo es integrar en un solo ejercicio varios conceptos vistos durante el curso:

- configuración de periféricos básicos
- comunicación I2C
- uso de interrupciones externas
- lectura analógica mediante ADC
- organización de un programa por modos de operación

Este archivo acompaña al **solucionario** del ejercicio final. La idea es que los alumnos intenten implementarlo primero por su cuenta y luego comparen su propuesta con una posible solución funcional.

---

## Objetivo funcional

El sistema debe mostrar tres modos en la pantalla LCD. Cada vez que se presiona el botón, el sistema cambia al siguiente modo:

### Modo 0
Muestra un mensaje inicial y un contador.

### Modo 1
Muestra el tiempo que el sistema lleva activo, en segundos.

### Modo 2
Muestra el valor leído desde el potenciómetro usando el ADC del ESP32.

---

## Componentes usados

- ESP32 DevKit
- LCD 16x2 con módulo I2C
- Pulsador
- Potenciómetro
- Cables de conexión

---

## Conexiones

### LCD I2C
- `SDA` → GPIO **21**
- `SCL` → GPIO **22**
- `VCC` → **5V**
- `GND` → **GND**

### Botón
- Un terminal → GPIO **18**
- Otro terminal → **GND**

### Potenciómetro
- `SIG` → GPIO **35**
- `VCC` → **3V3**
- `GND` → **GND**

---

## Conceptos que se aplican

### 1. Interrupciones
El botón no se lee con `digitalRead()` dentro del `loop()` como en un programa tradicional, sino que se usa una **interrupción externa**.

Cuando el usuario presiona el botón:

- el ESP32 detecta un cambio de estado en el pin
- se ejecuta una rutina de interrupción
- esta rutina activa una bandera
- luego, en el `loop()`, el programa cambia de modo

Esto permite introducir a los estudiantes al uso básico de interrupciones sin sobrecargar la ISR con demasiadas tareas.

### 2. ADC
El potenciómetro se conecta a un pin analógico del ESP32 y se lee con `analogRead()`.

El valor obtenido permite mostrar en pantalla:

- el valor digital leído
- una aproximación del voltaje
- o una representación porcentual si se desea ampliar el ejercicio

### 3. LCD con I2C
La pantalla LCD permite visualizar el estado del sistema.  
En este ejercicio se usa para mostrar información distinta según el modo activo.

### 4. Lógica por modos
El programa está organizado en tres modos.  
Esto ayuda a que los estudiantes practiquen una estructura muy común en sistemas embebidos:

- una variable de estado
- cambios de estado por evento
- actualización de la salida según el estado actual

---

## Funcionamiento general del solucionario

1. Al iniciar, el ESP32 configura:
   - la pantalla LCD
   - el botón como entrada con `INPUT_PULLUP`
   - el pin del potenciómetro como entrada analógica
   - la interrupción del botón

2. El sistema comienza en el **Modo 0**.

3. Cada vez que se presiona el botón:
   - se genera una interrupción
   - se activa una bandera
   - el programa cambia al siguiente modo

4. El `loop()` se encarga de:
   - actualizar el contador en Modo 0
   - calcular el tiempo activo en Modo 1
   - leer el ADC del potenciómetro en Modo 2
   - mostrar la información correspondiente en el LCD

---

## Sobre el parpadeo de la pantalla

En esta **primera versión** del solucionario, la pantalla puede presentar un **parpadeo visible**.

Esto ocurre porque el programa:

- limpia la pantalla con frecuencia
- vuelve a escribir todo el contenido muchas veces

Ese comportamiento es útil como punto de partida didáctico porque permite discutir una mejora importante:

- **no siempre conviene refrescar toda la pantalla**
- una versión optimizada actualiza solo lo necesario

En otras palabras, este solucionario inicial es funcional, pero también sirve para que los alumnos detecten posibles mejoras.

---

## Qué se espera que el alumno practique

Con este ejercicio final, el alumno debería ser capaz de:

- cablear correctamente los dispositivos
- configurar una pantalla LCD I2C
- usar una interrupción externa con un botón
- leer una entrada analógica con el ADC del ESP32
- implementar una máquina de estados simple
- mostrar información dinámica en una interfaz básica

---

## Posibles mejoras futuras

Una vez logrado el funcionamiento básico, el proyecto puede extenderse con:

- antirrebote más robusto para el botón
- actualización parcial del LCD para evitar parpadeo
- visualización del tiempo en formato `mm:ss`
- barra gráfica del potenciómetro en la pantalla
- almacenamiento del modo actual
- uso de tareas o temporizadores

---

## Observación final

Este solucionario muestra una forma válida y clara de resolver el ejercicio, priorizando que los conceptos del curso sean visibles en el código.  
No necesariamente es la versión más optimizada, pero sí una buena base para aprender, explicar y luego refinar.
