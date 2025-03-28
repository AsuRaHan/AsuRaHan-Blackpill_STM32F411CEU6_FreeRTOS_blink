#include "stm32f4xx.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"

#include "FreeRTOS.h"
#include "task.h"

// Конфигурация системного тактирования
void SystemClock_Config(void) {
    LL_RCC_HSE_Enable(); // Включение HSE
    while(!LL_RCC_HSE_IsReady()); // Ожидание готовности HSE
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 200, LL_RCC_PLLP_DIV_2); // Настройка PLL
    LL_RCC_PLL_Enable(); // Включение PLL
    while(!LL_RCC_PLL_IsReady()); // Ожидание готовности PLL
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3); // Настройка задержки Flash
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1); // Настройка делителя AHB
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2); // Настройка делителя APB1
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1); // Настройка делителя APB2
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL); // Установка источника системного тактирования
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL); // Ожидание установки источника
    SystemCoreClock = 100000000; // Установка частоты ядра
    LL_Init1msTick(SystemCoreClock); // Инициализация таймера
    LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLQ_DIV_4); // Настройка PLL для 48 МГц
}

// Инициализация светодиодов (PC13 и PA5)
void LED_Init(void) {
    // PC13 (встроенный светодиод, активный низкий уровень)
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); // Включение тактирования порта C
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT); // Установка режима вывода
    LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL); // Установка типа вывода
    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_HIGH); // Установка скорости

    // PA5 (внешний светодиод, активный высокий уровень)
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Включение тактирования порта A
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT); // Установка режима вывода
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL); // Установка типа вывода
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH); // Установка скорости
}

// Задача для моргания светодиодом на PC13 (активный низкий)
void LedBlinkTaskPC13(void *pvParameters) {
    (void)pvParameters; // Игнорируем параметр
    while (1) {
        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13); // Включение светодиода (низкий уровень)
        vTaskDelay(pdMS_TO_TICKS(1)); // Задержка 1 мс
        LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);   // Выключение светодиода (высокий уровень)
        vTaskDelay(pdMS_TO_TICKS(999)); // Задержка 999 мс
    }
}

// Задача для моргания светодиодом на PA5 (активный высокий)
void LedBlinkTaskPA5(void *pvParameters) {
    (void)pvParameters; // Игнорируем параметр
    while (1) {
        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);    // Включение свет
        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);    // Включение светодиода (высокий уровень)
        vTaskDelay(pdMS_TO_TICKS(500)); // Задержка 500 мс
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);  // Выключение светодиода (низкий уровень)
        vTaskDelay(pdMS_TO_TICKS(500)); // Задержка 500 мс
    }
}

// Заглушка для Idle Hook
void vApplicationIdleHook(void) {
    // Пустая заглушка, можно использовать для отладки или низкоприоритетных задач
}

int main(void) {
    SystemClock_Config(); // Настройка системного тактирования
    LED_Init(); // Инициализация светодиодов

    // Создание задачи для PC13
    xTaskCreate(
        LedBlinkTaskPC13, // Указатель на функцию задачи
        "LedPC13", // Имя задачи
        configMINIMAL_STACK_SIZE, // Размер стека
        NULL, // Параметры задачи
        2, // Приоритет задачи
        NULL // Указатель на дескриптор задачи
    );

    // Создание задачи для PA5
    xTaskCreate(
        LedBlinkTaskPA5, // Указатель на функцию задачи
        "LedPA5", // Имя задачи
        configMINIMAL_STACK_SIZE, // Размер стека
        NULL, // Параметры задачи
        2, // Приоритет задачи
        NULL // Указатель на дескриптор задачи
    );

    vTaskStartScheduler(); // Запуск планировщика FreeRTOS

    for(;;); // Бесконечный цикл, если планировщик не запустится
}
