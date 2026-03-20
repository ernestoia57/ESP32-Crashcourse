# Curso de ESP32 con PlatformIO y Wokwi

Bienvenido/a al repositorio del curso. Aquí encontrarás los ejemplos, ejercicios y proyectos base trabajados a lo largo de las sesiones, usando **ESP32**, **PlatformIO en Visual Studio Code** y **Wokwi** para simulación.

Este material ha sido organizado para que puedas:

- revisar ejemplos funcionales vistos en clase
- entender la estructura de un proyecto embebido con ESP32
- practicar periféricos y conceptos fundamentales
- simular los circuitos localmente antes de llevarlos a hardware real

---

## Sobre el autor

**Ernesto Esteban Isidro Acero**  
Estudiante de la **Maestría en Computer Science** en la **PUCP**  
**B.Sc. en Ingeniería Electrónica** por la **PUCP**  
Investigador en el **Digital Signal Processing Laboratory**

**LinkedIn:** [https://www.linkedin.com/in/eisidro/](https://www.linkedin.com/in/eisidro/)  
**Correo institucional:** e.isidro@pucp.edu.pe  
**Correo personal:** ernestoia57@gmail.com

---

## Tecnologías y entorno de trabajo usado en el curso

Para el desarrollo del curso se trabajó con las siguientes herramientas:

- **ESP32**
- **Visual Studio Code**
- **PlatformIO**
- **Wokwi**

### ¿Por qué estas herramientas?
Se eligió este entorno porque permite trabajar de forma ordenada y cercana a un flujo de desarrollo real:

- **PlatformIO** facilita la compilación, gestión de librerías y organización del proyecto
- **Wokwi** permite simular circuitos sin necesidad de tener el hardware físicamente conectado
- **VS Code** integra todo en un solo espacio de trabajo

---

## Temas trabajados durante el curso

A lo largo del curso se desarrollaron distintos temas fundamentales de sistemas embebidos con ESP32. Entre ellos:

### 1. Entradas y salidas digitales
Se trabajó la lógica básica de control de pines digitales, incluyendo activación de LEDs, lectura de pulsadores y estructuras básicas en `setup()` y `loop()`.

### 2. Interrupciones
Se introdujo el uso de interrupciones externas para detectar eventos de botones de manera más eficiente que con polling tradicional. Se revisó la idea de ISR, banderas de control y buenas prácticas para mantener rutinas de interrupción cortas.

### 3. Comunicación I2C
Se usó el bus I2C para interactuar con dispositivos externos, especialmente pantallas LCD con adaptador I2C y sensores.

### 4. Pantallas LCD
Se mostró cómo desplegar información al usuario en una pantalla LCD 16x2, organizando texto, variables y estados del sistema.

### 5. Lectura analógica con ADC
Se revisó cómo capturar señales analógicas desde sensores o potenciómetros usando los canales ADC del ESP32.

### 6. PWM
Se trabajó la generación de señales PWM para control de salida variable, introduciendo frecuencia, duty cycle y aplicaciones típicas.

### 7. Sensores
Se utilizó el sensor **INA219** como ejemplo de medición de voltaje y corriente, integrando lectura de datos y visualización.

### 8. Integración de periféricos
En varios ejemplos se combinaron múltiples conceptos, como interrupciones, pantalla LCD y ADC, para construir aplicaciones más completas.

### 9. Simulación embebida
Se desarrolló una metodología de prueba local usando **Wokwi**, permitiendo validar conexiones y comportamiento del sistema antes de la implementación física.

---

## Estructura del repositorio

El repositorio contiene varias carpetas, cada una asociada a un tema o proyecto específico trabajado en el curso.

### `esp32-ADC-PWM`
Contiene ejemplos relacionados con:

- lectura analógica mediante ADC
- uso de potenciómetros o señales variables
- generación de PWM
- análisis básico de cómo una entrada analógica puede influir sobre una salida modulada

Este material es útil para entender cómo capturar y transformar señales dentro del ESP32.

---

### `esp32-INO219`
Contiene ejemplos de uso del sensor **INA219**, orientados a:

- medición de voltaje
- medición de corriente
- lectura de variables eléctricas desde el ESP32
- uso de librerías externas en PlatformIO

Esta carpeta sirve como introducción al trabajo con sensores I2C y monitoreo eléctrico básico.

---

### `esp32-INT-LCD`
Contiene un proyecto que integra:

- interrupciones con botón
- pantalla LCD I2C
- lógica por modos
- lectura ADC

Es uno de los ejemplos más completos del curso, ya que combina varios de los temas trabajados y los organiza como una pequeña interfaz embebida.

---

### `esp32-INT`
Contiene ejemplos básicos de interrupciones, por ejemplo:

- activación por pulsador
- detección de eventos mediante ISR
- control simple de secuencias o estados

Es una carpeta importante para comprender la diferencia entre polling y eventos por hardware.

---

### `esp32-ntp-clock-main`
Contiene un ejemplo orientado al uso del ESP32 para obtener y mostrar hora mediante sincronización de red, introduciendo conceptos como:

- conexión a WiFi
- sincronización horaria
- uso de NTP
- aplicaciones con información temporal

Este proyecto permite ver una aplicación más cercana a sistemas conectados o IoT.

---

## Recomendación importante para los estudiantes

Cada carpeta contiene su propio propósito, estructura y ejemplos.  
**Si tienes dudas sobre un proyecto específico, revisa primero el `README` de esa carpeta.**

Los `README` han sido pensados como guía de apoyo para entender:

- qué hace cada proyecto
- qué conexiones se usan
- qué conceptos se ponen en práctica
- cómo compilar o simular cada ejemplo

Antes de modificar o ejecutar un proyecto, conviene leer su documentación.

---

## Flujo de trabajo recomendado

Para trabajar correctamente con este repositorio, se recomienda:

1. abrir la carpeta del proyecto deseado en **Visual Studio Code**
2. revisar su `platformio.ini`
3. leer su `README`
4. compilar el proyecto
5. simularlo en Wokwi o probarlo en hardware real

---

## Guía breve para simular con Wokwi localmente

Durante el curso se trabajó con simulación local usando **Wokwi** sobre proyectos de **PlatformIO**.

### Archivos típicos necesarios
Para simular un proyecto localmente, normalmente se requiere:

- `diagram.json` → define los componentes y conexiones
- `wokwi.toml` → indica a Wokwi dónde está el firmware compilado
- `platformio.ini` → configuración del entorno de compilación

### Paso 1: compilar el proyecto
Antes de simular, debes compilar el código para generar el firmware. Desde la terminal del proyecto:

```bash
pio run
```
Esto generará los archivos `.bin` y `.elf` dentro de `.pio/build/...`

### Paso 2: crear o revisar el archivo `wokwi.toml`

En la raíz del proyecto debe existir un archivo llamado:

```TOML
[wokwi]
version = 1
firmware = '.pio/build/esp32dev/firmware.bin'
elf = '.pio/build/esp32dev/firmware.elf'
```
> El nombre `esp32dev` debe coincidir con el entorno definido en tu `platformio.ini`.
> Si tu entorno tiene otro nombre, debes reemplazarlo en esas rutas.

### Paso 3: verificar `diagram.json`

También debe estar en la raíz del proyecto el archivo `diagram.json`, que contiene el circuito de simulación.

### Paso 4: iniciar la simulación

Una vez compilado el firmware y con los archivos de configuración listos, puedes iniciar la simulación desde Wokwi en VS Code.

---

## Sobre la organización del material

El material de este curso ha sido preparado con fines formativos y prácticos. La idea no es solo mostrar código funcional, sino también ofrecer ejemplos que sirvan como base para:

* experimentar
* modificar
* extender
* comparar distintas soluciones

Se recomienda a los estudiantes no limitarse a ejecutar los ejemplos, sino intentar comprender:

* qué periférico se configura
* qué flujo lógico sigue el programa
* por qué se eligió cierta estructura de código
* qué mejoras podrían hacerse

---

## Buenas prácticas sugeridas

Durante el desarrollo de los proyectos del curso, conviene tener en cuenta algunas buenas prácticas:

* comentar el código de forma clara
* separar configuración, lógica e impresión de resultados
* evitar sobrecargar las rutinas de interrupción
* revisar bien los pines usados en el ESP32
* validar conexiones antes de compilar
* simular antes de probar en hardware real cuando sea posible

---

## Derechos de autor y uso del material

Todo el material contenido en este repositorio ha sido preparado para fines académicos y de formación.

**Derechos de autor reservados.**
Este contenido no debe ser redistribuido, republicado ni comercializado total o parcialmente sin autorización expresa del autor.

Se permite su uso como material de estudio dentro del marco del curso, respetando siempre la autoría correspondiente.

---

## Mensaje final

Espero que este repositorio te sirva no solo como registro el curso, sino también como punto de partida para futuros proyectos con el ESP32.

Lee con calma los ejemplos, revisa los `README` de cada carpeta, prueba las simulaciones, pero sobre todo, experimenta. 
Recuerd siempre que la curiosidad es la semilla del conociemiento. Atentamente, Ernesto Isidro. 
