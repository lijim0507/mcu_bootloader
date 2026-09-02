===========================================================
  APM32F402 RTOS Getting Started Demo v1.0
===========================================================

  MCU:  Geehy APM32F402/403 (Cortex-M4, 128KB Flash, 32KB RAM)
  Board: APM32F402/403 Tiny Board
  OS:   CMSIS-FreeRTOS (CMSIS RTOS2 API)

===========================================================
  Features demonstrated
===========================================================

  - FreeRTOS: Multi-threading with 3 concurrent threads
  - GPIO:   LED2 fast blink (200ms), LED3 slow blink (500ms)
  - USART:  Serial printf output (USART1, PA9/PA10, 115200-8N1)
  - Button: KEY1 & KEY2 GPIO polling with software debounce
  - Clock:  HSE -> PLL @ 120MHz
  - RTOS:   osDelay() kernel-aware timing, osThreadNew()

===========================================================
  Thread architecture
===========================================================

  Led_Thread (osPriorityNormal, 512 bytes stack):
    - LED2 toggle every 200ms
    - LED3 toggle every 500ms
    - Uses osDelay(10) for 10ms tick

  Usart_Thread (osPriorityNormal, 512 bytes stack):
    - Prints counter and uptime every 1000ms
    - Uses osDelay(1000)

  Button_Thread (osPriorityNormal, 512 bytes stack):
    - Scans KEY1 and KEY2 every 50ms
    - Software debounce with edge detection
    - KEY1: Toggle LED2, print message
    - KEY2: Toggle LED3, print status

===========================================================
  Project structure (fully self-contained)
===========================================================

APM32F402_RTOS/
  |-- Application/                <-- Your application code
  |   |-- Include/
  |   |   |-- main.h
  |   |   |-- FreeRTOSConfig.h
  |   |   `-- apm32f402_403_int.h
  |   `-- Source/
  |       |-- main.c
  |       `-- apm32f402_403_int.c
  |
  |-- BSP/                        <-- Board Support Package
  |-- Libraries/                  <-- MCU libraries (CMSIS, StdPeriph, Device)
  |
  |-- Middlewares/                 <-- Middleware
  |   `-- CMSIS-FreeRTOS/         <-- FreeRTOS kernel + CMSIS RTOS2
  |       |-- CMSIS/RTOS2/
  |       `-- Source/
  |
  |-- Project/                    <-- IDE project files
  |   |-- MDK/                    (Keil uVision 5)
  |   |-- IAR/                    (IAR EWARM)
  |   `-- Eclipse/                (Eclipse CDT + GCC)
  |
  |-- Makefile                    <-- GCC command-line build
  |-- flash.jlink                 <-- J-Link flash script
  |-- readme.txt                  <-- This file
  `-- Clean Project.bat

===========================================================
  Quick start - GCC (command line)
===========================================================

  Prerequisites:
    ARM GNU Toolchain: https://developer.arm.com/downloads/-/gnu-rm

  Build:
    cd APM32F402_RTOS
    make

  Flash (J-Link):
    make flash

  Clean:
    make clean

===========================================================
  Hardware connections
===========================================================

  LED2:  PB6 (active-low: write BC to turn ON)
  LED3:  PB7 (active-low)
  KEY1:  PA1 (pull-up, active-low when pressed)
  KEY2:  PA0 (pull-up, active-low when pressed)
  COM1:  USART1  TX=PA9, RX=PA10

  Serial settings: 115200-8-N-1

===========================================================
  Expected serial output
===========================================================

========================================
  APM32F402 RTOS Demo v1.0
========================================
  OS:  CMSIS-FreeRTOS (CMSIS RTOS2)
  Threads: LED, USART, Button
========================================

[0001] System running OK, uptime=1000ms
[0002] System running OK, uptime=2000ms
  -> KEY1 pressed! Toggling LED2
  -> KEY2 pressed! System running OK
...

===========================================================
  Key configuration macros
===========================================================

  Defined in Project settings / Makefile:
    APM32F402xx              - MCU selection
    BOARD_APM32F402_403_TINY - Board selection
    USE_STDPERIPH_DRIVER      - Enable StdPeriph driver

  Defined in FreeRTOSConfig.h:
    configTOTAL_HEAP_SIZE  4096  - Heap size (bytes)
    configTICK_RATE_HZ     1000  - Kernel tick rate (Hz)
    configMINIMAL_STACK_SIZE 128 - Minimum task stack (words)
