# Smart Lock Access Control System (ESP32-S3)

**Course:** Sistemas Embarcados (Embedded Systems)  
**Institution:** Universidade Federal do Espírito Santo (UFES)  
**Student:** Miguel Catelan Magalhães  
**Date:** 2026/1

## 1. Introduction
This project implements a **Smart Lock System** using the **ESP32-S3** microcontroller. The system simulates a secure access control mechanism featuring a numeric keypad, an LCD interface, and a magnetic lock control.

[cite_start]The firmware was developed in C++ (Arduino framework) focusing on hardware manipulation without external libraries for the peripherals, ensuring precise timing and control[cite: 253, 254].

## 2. Features
* [cite_start]**Authentication:** Supports multiple user passwords (minimum 4 digits) and one Administrator password[cite: 224, 226].
* [cite_start]**Admin Mode:** Allows the administrator to register, delete, and modify user passwords directly via the interface[cite: 227].
* **Hardware Interface:**
    * [cite_start]**3x4 Matrix Keypad:** For password entry and menu navigation[cite: 192].
    * [cite_start]**16x2 LCD Display:** Provides user feedback (masked passwords, access granted/denied messages)[cite: 228].
* [cite_start]**Adaptive Backlight:** The LCD backlight intensity adjusts automatically based on ambient light measured by an LDR sensor (PWM control)[cite: 243, 245].
* [cite_start]**Lock Control:** Controls a solenoid lock (simulated by LED) via a relay/transistor driver[cite: 203].

## 3. Hardware Architecture
The project is designed to run on a custom PCB provided by the course, which includes protection buffers and drivers.

### Key Components
* **Microcontroller:** ESP32-S3
* [cite_start]**Buffer:** 74HC245 (voltage follower for protection) [cite: 193]
* [cite_start]**Driver:** TIP122 Transistor (for the magnetic lock) [cite: 203]
* **Sensor:** LDR (Light Dependent Resistor)

### Pinout Mapping
Based on the project schematic:

| Component       | ESP32-S3 GPIO | Description                     |
| :---            | :---:         | :---                            |
| **Lock (Output)** | `47`        | Activates the solenoid/LED      |
| **LDR (Input)** | `--`         | Analog input for light sensing  |
| **Backlight** | `--`        | PWM Output for LCD brightness   |
| **Keypad Col 1** | `36`        | Output (Scanning)               |
| **Keypad Col 2** | `37`        | Output (Scanning)               |
| **Keypad Col 3** | `38`        | Output (Scanning)               |
| **Keypad Row 1** | `48`        | Input (Read)                    |
| **Keypad Row 2** | `45`        | Input (Read)                    |
| **Keypad Row 3** | `0`         | Input (Read)                    |
| **Keypad Row 4** | `39`         | Input (Read)                    |
| **LCD RS** | `13`         | Register Select                 |
| **LCD E** | `14`         | Enable                          |
| **LCD D4-D7** | `4,5,6,7`     | 4-bit Data Bus                  |

## 4. How It Works
1.  **Initialization:** The system initializes the LCD in 4-bit mode and configures the GPIOs.
2.  **Standby:** The screen displays a welcome message. The backlight adjusts dynamically.
3.  **Input:** The user enters a password. Characters are masked (`*`) on the screen.
4.  **Verification:**
    * [cite_start]**Correct Password:** The lock opens (GPIO 35 HIGH) for 3 seconds[cite: 229].
    * **Incorrect Password:** "Access Denied" message is shown.
    * **Admin Code:** Enters the configuration menu to manage users.

## 5. Simulation (Wokwi)
Since the physical hardware includes a **74HC245 buffer** (transparent to software) and a **TIP122 transistor**, the simulation in Wokwi uses simplified components:
* **Lock:** Represented by a Green LED.
* **Backlight:** Represented by a Yellow LED driven by PWM.
* **LDR:** Simulated using a Potentiometer.

## 6. How to Run
1.  Clone this repository.
2.  Open the project in **VS Code** with the **PlatformIO** extension (or Arduino IDE).
3.  Verify the Pinout definitions in `main.cpp` match your hardware.
4.  Upload the code to the ESP32-S3.
