# ESP32 ESP-IDF Examples - Embedded Systems

Collection of codes and practical examples focused on embedded systems development using the **ESP32** microcontroller and the official **ESP-IDF** (Espressif IoT Development Framework) framework.

---

## Repository Structure and Contents

The examples are organized by categories according to complexity level and the peripherals used:

### Fundamentals & System (FreeRTOS)
* **`Hello_world`**: Base project for initialization and system testing with ESP-IDF.
* **`tipos_de_datos`**: Basic handling of data types and variables in C/C++ for microcontrollers.
* **`Ejemplo_task`**: Creation and management of tasks using FreeRTOS in ESP-IDF.
* **`xTimer_leds`**: Use of software timers (`xTimer`) for asynchronous LED control.

### Peripherals & GPIOs
* **`pines_digitales_entrada`**: Digital pin reading (configuration of inputs with pull-up/pull-down resistors).
* **`pines_digitales_salida`**: Basic digital output control for device actuation.
* **`pines_analogicos`**: Reading analog channels using the internal ADC of the ESP32.
* **`pines_PWM`**: Generation of PWM signals (LEDC modules) for dimming lights or controlling motors.
* **`interrupciones_externas`**: Configuration of Interrupt Service Routines (ISRs) triggered by state changes on external pins.
* **`interrupciones_internas`**: Use of internal hardware interrupts (such as hardware timers).

### Sensors & Displays
* **`DS3231`**: I2C communication and reading date/time from the DS3231 Real-Time Clock module.
* **`pantalla_mini_oled_i2c`**: Control of compact OLED displays via the I2C protocol.
* **`regleta_test`**: Testing and validation of auxiliary hardware or connection terminal strips.

### Dual Core
* **`dual_core`**: Implementation and distribution of dedicated tasks running on Core 0 and Core 1 of the ESP32.

### Connectivity & Web Servers
* **`DUALCOREWEBSERVER`**: Advanced web server running in parallel with multi-core task processing.
* **`ServidorNTC`**: Basic implementation of an embedded web server on the ESP32.
* **`ServidorNTC2`**: Extended or alternative version for managing web requests and reading sensors.
