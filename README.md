# Мигание светодиодами на STM32F4 с FreeRTOS

Этот проект демонстрирует работу двух светодиодов на плате STM32F4 с использованием операционной системы реального времени FreeRTOS. Проект реализует две независимые задачи, которые управляют миганием встроенного и внешнего светодиодов. 

Проект построен на основе **HAL** (Hardware Abstraction Layer) и **LL** (Low-Layer) библиотек от STMicroelectronics, что позволяет эффективно управлять периферией микроконтроллера. 

Для сборки и загрузки прошивки используется **PlatformIO**, что обеспечивает простоту и удобство в управлении зависимостями и настройками проекта. Настройки в файле `platformio.ini` автоматически используют последние версии библиотек, что гарантирует актуальность и совместимость с последними обновлениями.

Файл `platformio.ini` содержит следующие ключевые параметры:

- **platform**: Указывает на репозиторий платформы STM32.
- **board**: Определяет используемую плату (в данном случае `blackpill_f411ce`).
- **framework**: Указывает на использование фреймворка STM32Cube.
- **build_flags**: Настройки компиляции, включая параметры для использования FreeRTOS и настройки PLL для достижения частоты 100 МГц.

## Основные характеристики

- **Микроконтроллер**: STM32F4xx
- **Операционная система**: FreeRTOS
- **Язык программирования**: C
- **Инструментарий**: Стандартная библиотека STM32 (LL)

## Функциональность

Проект реализует две независимые задачи:

- Мигание встроенным светодиодом (PC13) с периодом 1 секунда (1 мс включен, 999 мс выключен).
- Мигание внешним светодиодом (PA5) с периодом 1 секунда (500 мс включен, 500 мс выключен).

## Конфигурация системы

### Тактирование

- Используется внешний кварцевый генератор (HSE).
- Настроен PLL для получения частоты ядра 100 МГц.
- Настройка делителей для шин APB1 и APB2.

### Периферия

- Инициализированы два GPIO-пина:
  - **PC13**: встроенный светодиод, активный уровень - низкий.
  - **PA5**: внешний светодиод, активный уровень - высокий.

## Структура проекта

- `SystemClock_Config()` - настройка системного тактирования.
- `LED_Init()` - инициализация GPIO для управления светодиодами.
- `LedBlinkTaskPC13()` - задача для мигания светодиодом на PC13.
- `LedBlinkTaskPA5()` - задача для мигания светодиодом на PA5.
- `main()` - создание задач и запуск планировщика FreeRTOS.

## Настройка задач FreeRTOS

- Обе задачи создаются с одинаковым приоритетом (2).
- Используется минимальный размер стека (`configMINIMAL_STACK_SIZE`).
- Задачи не передают параметры и не используют механизмы синхронизации.

## Особенности

- Встроенный светодиод (PC13) имеет инвертированную логику (активный уровень - низкий).
- Внешний светодиод (PA5) использует стандартную логику (активный уровень - высокий).
- Для задержек используются функции FreeRTOS (`vTaskDelay`).

## Компиляция и загрузка

1. Убедитесь, что у вас установлен [PlatformIO](https://platformio.org/) в вашей среде разработки (например, VSCode).
2. Откройте проект в PlatformIO.
3. Скомпилируйте проект, нажав на кнопку "Build" в нижней панели или используя команду `PlatformIO: Build` из командной палитры.
4. Подключите плату STM32F4 к компьютеру.
5. Загрузите прошивку на плату, нажав на кнопку "Upload" в нижней панели или используя команду `PlatformIO: Upload` из командной палитры.

## Тестирование

После загрузки прошивки светодиоды должны начать мигать с заданными периодами:

- Встроенный светодиод (PC13) будет мигать с периодом 1 секунда.
- Внешний светодиод (PA5) — с периодом 500 мс.

## Заключение

Теперь у вас есть работающий проект для мигания светодиодами на STM32F4 с использованием FreeRTOS. Вы можете расширить функциональность, добавив дополнительные задачи или изменив логику работы светодиодов.

## Лицензия

Ни кому не разрешаю использовать этот код, в комерчиских а особенно в военных целях.
