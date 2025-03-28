# Мигание светодиодами на STM32F4 с FreeRTOS

Этот проект демонстрирует работу двух светодиодов на плате STM32F4 с использованием операционной системы реального времени FreeRTOS. Проект реализует две независимые задачи, которые управляют миганием встроенного и внешнего светодиодов.

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

Этот проект лицензирован под MIT License. См. файл [LICENSE](LICENSE) для получения дополнительной информации.
