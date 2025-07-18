# Multi-threaded Array Marker System

## Постановка задачи

Программа реализует систему с главным потоком (`main`) и несколькими потоками-маркерами (`marker`), которые взаимодействуют через общий массив целых чисел.

### Функционал потоков

**Поток main**:
1. Инициализирует массив нулями (размер вводится с консоли)
2. Создает указанное количество потоков `marker`
3. Координирует работу потоков через систему сигналов
4. Обрабатывает запросы потоков на невозможность продолжения работы
5. Управляет завершением работы потоков
6. Выводит состояние массива на разных этапах работы

**Поток marker** (каждый экземпляр):
1. Начинает работу по сигналу от `main`
2. Помечает случайные элементы массива своим номером
3. При невозможности пометки элемента сообщает об этом `main`
4. Ожидает решения от `main` (продолжить или завершиться)
5. При завершении обнуляет все свои пометки в массиве

## Используемые инструменты

### Система сборки
- **CMake** (минимальная версия 3.10)
- Поддержка многопоточных приложений

### Тестирование
- **Google Test (gtest)** для модульного тестирования
- Тестовые сценарии:
  - Корректность инициализации массива
  - Взаимодействие потоков
  - Механизм пометки элементов
  - Процедура завершения потоков
  - Обработка граничных случаев

### Компилятор
- Требуется поддержка C++17 или новее
- Необходима поддержка:
  - Многопоточности (`std::thread`)
  - Синхронизации (`std::mutex`, `std::condition_variable`)
  - Атомарных операций

### Платформа
- Изначально разработано для Windows (использует WinAPI `Sleep`)
- Может быть портировано на другие ОС с заменой функций задержки

## Сборка и запуск

1. Создайте директорию сборки:
   ```bash
   mkdir build && cd build
   ```

2. Настройте проект:
   ```bash
   cmake ..
   ```

3. Соберите проект:
   ```bash
   cmake --build .
   ```

4. Запустите программу:
   ```bash
   ./marker_system
   ```

## Структура проекта

```
marker_system/
├── CMakeLists.txt            # Основная конфигурация CMake
├── include/
│   ├── shared_array.h        # Класс для работы с разделяемым массивом
│   ├── thread_manager.h      # Управление потоками
│   ├── marker_thread.h       # Реализация потока-marker
│   └── main_utils.h          # Вспомогательные функции
├── src/
│   ├── main.cpp              # Главный поток
│   ├── marker_thread.cpp     # Логика потока-marker
│   ├── shared_array.cpp      # Реализация разделяемого массива
│   └── thread_manager.cpp    # Управление потоками
├── test/
│   ├── CMakeLists.txt        # Конфигурация тестов
│   ├── test_shared_array.cpp # Тесты разделяемого массива
│   ├── test_marker.cpp       # Тесты потока-marker
│   └── test_integration.cpp  # Интеграционные тесты
└── README.md                 # Этот файл
```

## Особенности реализации

1. **Синхронизация**:
   - Используются мьютексы и условные переменные для координации потоков
   - Реализован механизм сигналов между потоками

2. **Разделяемый ресурс**:
   - Класс `SharedArray` обеспечивает безопасный доступ к массиву
   - Используются атомарные операции для критических секций

3. **Управление потоками**:
   - Каждый поток `marker` получает уникальный идентификатор
   - Реализованы состояния потоков (работает/ожидает/завершен)

4. **Обработка ошибок**:
   - Проверка ввода пользователя
   - Обработка исключительных ситуаций
   - Защита от взаимоблокировок

5. **Производительность**:
   - Минимальные блокировки для обеспечения параллелизма
   - Оптимизированные циклы ожидания

## Пример работы

1. Пользователь вводит размер массива (например, 10)
2. Вводит количество потоков (например, 3)
3. Программа выводит начальное состояние массива (все нули)
4. Потоки начинают помечать элементы:
   ```
   Thread 2 marked element at index 4
   Thread 1 marked element at index 7
   Thread 3 can't mark element at index 4 (already marked)
   ```
5. `main` выводит текущее состояние массива
6. Пользователь выбирает поток для завершения (например, 2)
7. Программа выводит массив после завершения потока 2 (его пометки обнулены)
8. Процесс продолжается, пока все потоки не завершат работу
