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
    LL_RCC_HSE_Enable();
    while(!LL_RCC_HSE_IsReady());
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 200, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();
    while(!LL_RCC_PLL_IsReady());
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
    SystemCoreClock = 100000000;
    LL_Init1msTick(SystemCoreClock);
    LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLQ_DIV_4);
}

// Инициализация светодиодов (PC13 и PA5)
void LED_Init(void) {
    // PC13 (встроенный светодиод, активный низкий уровень)
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_HIGH);

    // PA5 (внешний светодиод, активный высокий уровень)
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
}

// Задача для моргания светодиодом на PC13 (активный низкий)
void LedBlinkTaskPC13(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13); // Вкл (низкий уровень)
        vTaskDelay(pdMS_TO_TICKS(1));
        LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);   // Выкл (высокий уровень)
        vTaskDelay(pdMS_TO_TICKS(999));
    }
}

// Задача для моргания светодиодом на PA5 (активный высокий)
void LedBlinkTaskPA5(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);    // Вкл (высокий уровень)
        vTaskDelay(pdMS_TO_TICKS(500));
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);  // Выкл (низкий уровень)
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Заглушка для Idle Hook
void vApplicationIdleHook(void) {
    // Пустая заглушка
}

int main(void) {
    SystemClock_Config();
    LED_Init();

    // Создание задачи для PC13
    xTaskCreate(
        LedBlinkTaskPC13,
        "LedPC13",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        NULL
    );

    // Создание задачи для PA5
    xTaskCreate(
        LedBlinkTaskPA5,
        "LedPA5",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        NULL
    );

    vTaskStartScheduler();
    for(;;);
}