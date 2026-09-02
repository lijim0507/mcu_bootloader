/* --------------------------------------------------------------------------
 * Copyright (c) 2013-2022 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 * --------------------------------------------------------------------------
 * Project:     CMSIS-FreeRTOS
 * Title:       FreeRTOS configuration definitions for APM32F402/403
 * --------------------------------------------------------------------------*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if (defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(__ICCARM__))
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

#ifndef CMSIS_device_header
#define CMSIS_device_header "apm32f402_403.h"
#endif

/*-----------------------------------------------------------
 * Application specific definitions.
 *----------------------------------------------------------*/
#define configMINIMAL_STACK_SIZE                ((uint16_t)(128))
#define configTOTAL_HEAP_SIZE                   ((size_t)4096)
#define configTICK_RATE_HZ                      ((TickType_t)1000)
#define configTIMER_TASK_STACK_DEPTH            80
#define configTIMER_TASK_PRIORITY               40
#define configTIMER_QUEUE_LENGTH                5
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    16
#define configUSE_TIME_SLICING                  1
#define configUSE_TICKLESS_IDLE                 0
#define configIDLE_SHOULD_YIELD                 1
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0
#define configUSE_MALLOC_FAILED_HOOK            0
#define configQUEUE_REGISTRY_SIZE               0

/* Event Recorder configuration */
#define configEVR_INITIALIZE                    1
#define configEVR_SETUP_LEVEL                   1
#define configEVR_LEVEL_TASKS                   0x05
#define configEVR_LEVEL_QUEUE                   0x05
#define configEVR_LEVEL_TIMERS                  0x05
#define configEVR_LEVEL_EVENTGROUPS             0x05
#define configEVR_LEVEL_HEAP                    0x05
#define configEVR_LEVEL_STREAMBUFFER            0x05

/* Port Specific Features */
#define configENABLE_FPU                        1
#define configENABLE_MVE                        0
#define configENABLE_MPU                        0
#define configRUN_FREERTOS_SECURE_ONLY          0
#define configENABLE_TRUSTZONE                  1
#define configMINIMAL_SECURE_STACK_SIZE         ((uint32_t)128)

/* CMSIS RTOS2 API required definitions (do not change!) */
#define configCPU_CLOCK_HZ                      (SystemCoreClock)
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configUSE_PREEMPTION                    1
#define configUSE_TIMERS                        1
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_TRACE_FACILITY                1
#define configUSE_16_BIT_TICKS                  0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configMAX_PRIORITIES                    56
#define configKERNEL_INTERRUPT_PRIORITY         255

/* CMSIS RTOS2 API function includes (do not change!) */
#define INCLUDE_xEventGroupSetBitsFromISR       1
#define INCLUDE_xSemaphoreGetMutexHolder        1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskDelayUntil                 1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xTimerPendFunctionCall          1

/* Assert and interrupt vector mapping */
#define configASSERT(x) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}

#define xPortPendSVHandler                      PendSV_Handler
#define vPortSVCHandler                         SVC_Handler
#define SysTick_Handler                         xPortSysTickHandler

#endif /* FREERTOS_CONFIG_H */
