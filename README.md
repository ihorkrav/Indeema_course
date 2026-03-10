| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-H21 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | --------- | -------- | -------- | -------- |
# ESP32-S3 Servo Control System

## Overview

This project implements a **servo control system for the ESP32-S3** using the **ESP-IDF framework**.
The servo can be controlled using either:

* a **joystick**
* **UART commands**

The current servo position is periodically **published to an MQTT broker**, enabling remote monitoring of the system.

The application is designed using **FreeRTOS tasks** and **mutex synchronization** to safely share data between components.

---

## Features

*  **Joystick control**

  * Analog joystick input controls servo angle (0–180°).
*  **UART control**

  * Servo angle can be sent via serial terminal (e.g. PuTTY).
  * When a UART command is received, joystick control is temporarily disabled.
* **Control mode switching**

  * Pressing the joystick button restores joystick control.
* **MQTT telemetry**

  * Servo position is periodically published to an MQTT topic.
* **LED status indicator**

  * Red LED → Joystick control mode
  * Blue LED → UART control mode
* **FreeRTOS multitasking**

  * Separate tasks handle UART, joystick, MQTT communication, and LED status.

---

## System Architecture

The application is built using **independent FreeRTOS tasks**:

| Task            | Description                                     |
| --------------- | ----------------------------------------------- |
| `uart_task`     | Reads servo angle commands from UART            |
| `joystick_task` | Reads joystick position and updates servo angle |
| `mqtt_task`     | Sends servo position data to the MQTT broker    |
| `led_task`      | Displays current control mode via LED           |

Shared data between tasks (`servo_angle` and `control_mode`) is protected using a **FreeRTOS mutex**.

---

## Control Logic

### Joystick Mode

* Default control mode
* Servo angle follows the joystick X-axis

### UART Mode

* Activated when a valid angle is received via UART
* Joystick input is ignored

### Returning to Joystick Mode

* Press the joystick button once

---

## MQTT Communication

```
To configure MQTT broker use menuconfig.
```

```
To configure WiFI settings use menucinfig.
```

Servo position is published as JSON:

```
{"servo":90}
```

Example topic:

```
esp-lection/servo
```

---

## Hardware Requirements

* ESP32-S3 development board
* Analog joystick module
* Servo motor
* WS2812 or compatible LED strip (status indicator)
* USB connection for UART communication

---

## Software Requirements

* **ESP-IDF v5.x**
* Python environment configured for ESP-IDF
* Serial terminal (PuTTY / minicom / screen)
* MQTT broker (example: `broker.hivemq.com`)

---

## Project Structure

```
components/
    UART/
    Joystick/
    Servo/
    MQTT/
    Wifi/
    LED/

main/
    main.c

CMakeLists.txt
sdkconfig
```

Each peripheral or feature is implemented as a **separate ESP-IDF component**.

---

## Build and Flash

### 1. Set ESP-IDF environment

```
. $HOME/esp/esp-idf/export.sh
```

### 2. Build the project

```
idf.py build
```

### 3. Flash firmware

```
idf.py flash
```

### 4. Open serial monitor

```
idf.py monitor
```

---

## UART Control

Connect using a serial terminal:

```
Baud rate: 115200
```

Send a number between:

```
0 - 180
```

Example:

```
90
```

The servo will move to the specified angle.

---

## Example Workflow

1. Move the joystick → servo moves
2. Send `120` via UART → servo moves to 120°
3. LED turns blue indicating **UART mode**
4. Press joystick button → LED turns red and joystick control resumes
5. Servo position is continuously sent to MQTT

---

## Future Improvements

* MQTT command control for remote servo positioning
* OTA firmware updates
* PID smoothing for joystick input
* Web interface for monitoring and control

---

## License

This project is intended for **educational purposes**.
