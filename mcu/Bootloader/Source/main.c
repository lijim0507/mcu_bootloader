/**
 * @file        main.c
 *
 * @brief       APM32F402/403 RTOS Getting Started Demo
 *              Demonstrates: FreeRTOS multi-threading (CMSIS RTOS2 API),
 *              GPIO (LED toggle), USART (serial printf), Button input,
 *              and kernel-aware timing with osDelay().
 *
 * @version     V1.0.0
 *
 * @date        2024-12-01
 *
 * @attention
 *
 *  Copyright (C) 2024-2025 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes ***************************************************************/
#include "main.h"
#include <stdio.h>

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
static osThreadId_t ledThreadID = NULL;
static osThreadId_t usartThreadID = NULL;
static osThreadId_t buttonThreadID = NULL;

static const osThreadAttr_t ledThreadAttr = {
    .name       = "LedToggle",
    .priority   = osPriorityNormal,
    .stack_size = 512
};

static const osThreadAttr_t usartThreadAttr = {
    .name       = "UsartPrint",
    .priority   = osPriorityNormal,
    .stack_size = 512
};

static const osThreadAttr_t buttonThreadAttr = {
    .name       = "ButtonScan",
    .priority   = osPriorityNormal,
    .stack_size = 512
};

/* External variables *****************************************************/

/*!
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 *
 * @note    The RTOS demo performs the following:
 *          1. Configure system clock (HSE -> PLL -> 120MHz)
 *          2. Initialize LEDs (LED2 + LED3)
 *          3. Initialize USART1 (115200 baud, 8N1, TX only)
 *          4. Initialize buttons (KEY1 + KEY2, GPIO polling mode)
 *          5. Initialize and start FreeRTOS kernel
 *          6. Create 3 threads:
 *             - Led_Thread: LED2 fast blink (200ms), LED3 slow blink (500ms)
 *             - Usart_Thread: Print counter and uptime every 1000ms
 *             - Button_Thread: Scan KEY1/KEY2 every 50ms with debounce
 */
int main(void)
{
    USART_Config_T USART_ConfigStruct;

    /* Step 1: System clock configuration */
    SystemClockConfig();

    /* Step 2: Initialize LEDs */
    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);

    /* Step 3: Initialize buttons (GPIO polling mode) */
    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_GPIO);
    BOARD_BUTTON_Config(BUTTON_KEY2, BUTTON_MODE_GPIO);

    /* Step 4: Initialize USART1 for printf */
    USART_ConfigStructInit(&USART_ConfigStruct);
    USART_ConfigStruct.baudRate     = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode         = USART_MODE_TX;
    USART_ConfigStruct.parity       = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits     = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength   = USART_WORD_LEN_8B;
    BOARD_COM_Config(COM1, &USART_ConfigStruct);

    /* Step 5: Print startup banner */
    printf("\r\n");
    printf("========================================\r\n");
    printf("  APM32F402 RTOS Demo v1.0             \r\n");
    printf("========================================\r\n");
    printf("  OS:  CMSIS-FreeRTOS (CMSIS RTOS2)    \r\n");
    printf("  Threads: LED, USART, Button          \r\n");
    printf("========================================\r\n\r\n");

    /* Step 6: Turn on LED2 as power-on indicator */
    BOARD_LED_On(LED2);

    /* Step 7: Initialize the RTOS kernel */
    osKernelInitialize();

    /* Step 8: Create threads */
    ledThreadID = osThreadNew(Led_Thread, NULL, &ledThreadAttr);
    if (ledThreadID == NULL)
    {
        printf("Error: Create Led_Thread failed!\r\n");
    }

    usartThreadID = osThreadNew(Usart_Thread, NULL, &usartThreadAttr);
    if (usartThreadID == NULL)
    {
        printf("Error: Create Usart_Thread failed!\r\n");
    }

    buttonThreadID = osThreadNew(Button_Thread, NULL, &buttonThreadAttr);
    if (buttonThreadID == NULL)
    {
        printf("Error: Create Button_Thread failed!\r\n");
    }

    /* Step 9: Start the RTOS scheduler (never returns) */
    osKernelStart();

    /* Should never reach here */
    while (1);
}

/**
 * @brief   LED toggle thread
 *
 * @param   argument  Not used
 *
 * @retval  None
 *
 * @note    LED2 fast blink (200ms), LED3 slow blink (500ms)
 */
void Led_Thread(void *argument)
{
    UNUSED(argument);

    uint32_t tick = 0;

    while (1)
    {
        /* LED2: Fast blink every 200ms */
        if ((tick % (LED_FAST_MS / 10)) == 0)
        {
            BOARD_LED_Toggle(LED2);
        }

        /* LED3: Slow blink every 500ms */
        if ((tick % (LED_SLOW_MS / 10)) == 0)
        {
            BOARD_LED_Toggle(LED3);
        }

        tick++;
        osDelay(10);
    }
}

/**
 * @brief   USART printf thread
 *
 * @param   argument  Not used
 *
 * @retval  None
 *
 * @note    Prints counter and uptime every 1000ms
 */
void Usart_Thread(void *argument)
{
    UNUSED(argument);

    uint32_t counter = 0;
    uint32_t uptime  = 0;

    while (1)
    {
        counter++;
        uptime += USART_PRINT_MS;

        printf("[%04lu] System running OK, uptime=%lums\r\n", counter, uptime);

        osDelay(USART_PRINT_MS);
    }
}

/**
 * @brief   Button scan thread
 *
 * @param   argument  Not used
 *
 * @retval  None
 *
 * @note    Scans KEY1 and KEY2 every 50ms with software debounce.
 *          KEY1: Toggle LED2 and print message
 *          KEY2: Toggle LED3 and print status
 */
void Button_Thread(void *argument)
{
    UNUSED(argument);

    uint8_t key1Prev = 0;
    uint8_t key2Prev = 0;

    while (1)
    {
        /* Check KEY1 */
        uint8_t key1Now = (BOARD_BUTTON_GetState(BUTTON_KEY1) == BIT_RESET) ? 1 : 0;
        if (key1Now && !key1Prev)
        {
            printf("  -> KEY1 pressed! Toggling LED2\r\n");
            BOARD_LED_Toggle(LED2);
        }
        key1Prev = key1Now;

        /* Check KEY2 */
        uint8_t key2Now = (BOARD_BUTTON_GetState(BUTTON_KEY2) == BIT_RESET) ? 1 : 0;
        if (key2Now && !key2Prev)
        {
            printf("  -> KEY2 pressed! System running OK\r\n");
            BOARD_LED_Toggle(LED3);
        }
        key2Prev = key2Now;

        osDelay(BUTTON_SCAN_MS);
    }
}

/* ================================================================
 *  printf redirection (compiler-specific implementation)
 * ============================================================== */
#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/**
 * @brief   Redirect printf to USART1 (Keil MDK / IAR)
 */
int fputc(int ch, FILE* f)
{
    UNUSED(f);
    USART_TxData(USART1, (uint8_t)ch);
    while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
    return (ch);
}

#elif defined (__GNUC__)

/**
 * @brief   Redirect printf to USART1 (GCC)
 */
int __io_putchar(int ch)
{
    USART_TxData(USART1, ch);
    while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
    return ch;
}

int _write(int file, char* ptr, int len)
{
    UNUSED(file);
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}

#else
#warning Not supported compiler type
#endif
