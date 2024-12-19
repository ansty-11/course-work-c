#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define POROG 500
#define WARNING_THRESHOLD 0.05 * POROG
#define MAX_TANKS 10 // Максимальное количество резервуаров
#define MAX_FILENAME_SIZE 20 // Максильное количество символов для названия файла
#define MAX_RECORD_COUNT 100 // МАксимальное количество сгенерированных записей

// Структура для хранения уровня жидкости и индекса резервуара
typedef struct {
    float level; // Уровень жидкости в резервуаре
    int index; // Индекс резервуара
} Record;

/**
 * Функция записывает данные о уровнях жидкости в файл.
 *
 * @param filename Имя файла для записи данных.
 * @param records Массив записей с данными о уровнях жидкости.
 * @param count Количество записей для записи.
 */
int data(const char* filename, Record* records, int count);

/*
 * Функция анализирует данные из файла для указанного резервуара.
 */
int analyz_data();

/**
 * Функция собирает данные о уровнях жидкости от пользователя.
 */
int collection_date();

/*
 * Функция генерирует случайные данные уровней жидкости для резервуаров.
 * @param records Указатель на массив записей для хранения сгенерированных данных.
 * @param count Указатель на переменную для хранения количества записей.
 */
int generate_random_data(Record** records, int* count);

/*
 * Функция сортирует записи по индексам резервуаров и уровням жидкости.
 *
 * @param records Массив записей для сортировки.
 * @param count Количество записей для сортировки.
 */
int sort_records(Record* records, int count);

/*
 * Функция записывает одну запись о уровне жидкости в файл.
 * @param file Указатель на файл для записи данных.
 * @param record Запись с данными о уровне жидкости.
 */
int write_record_to_file(FILE* file, Record record);

/*
 * Функция обрабатывает ввод данных от пользователя и сохраняет их в массив записей.
 * @param records Массив записей для сохранения введенных данных.
 * @param count Указатель на переменную для хранения количества записей.
 */
int handle_user_input(Record* records, int* count);

/*
 * Функция проверяет корректность формата ввода уровня жидкости и индекса резервуара.
 * @param input Строка с введенными данными от пользователя.
 * @param record Указатель на запись для сохранения правильных данных.
 */
int validate_input(const char* input, Record* record);

/*
 * Функция проверяет монотонность уровней жидкости для одного резервуара,
 * выводит предупреждение при нарушении монотонности.
 * @param records Массив записей с данными о уровнях жидкости.
 * @param count Количество записей для проверки монотонности.
 */
int check_monotonicity(Record* records, int count);

/*
 * Функция выводит средний уровень жидкости на экран.
 * @param total Сумма уровней жидкости для расчета среднего значения.
 * @param count Количество уровней для расчета среднего значения.
 */
int print_average_level(float total, int count);

/*
 * Функция выводит максимальный и минимальный уровни жидкости на экран.
 * @param max Максимальный уровень жидкости.
 * @param min Минимальный уровень жидкости.
 */
int print_max_min_levels(float max, float min);

int main() {
    setlocale(LC_ALL, "RUS"); //Локализация
    int choice;
    int monitoring = 0;

    while (1) {
        // Главное меню
        printf("1. Включить режим мониторинга\n");
        printf("2. Отключить режим мониторинга\n");
        printf("3. Анализ данных из файла\n");
        printf("4. Выйти\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: 
            monitoring = 1;
            while (monitoring) {
                // Подменю режима мониторинга
                printf("1. Ввод данных вручную\n");
                printf("2. Автоматическая генерация данных\n");
                printf("3. Вернуться в главное меню\n");
                printf("Выберите опцию: ");
                scanf("%d", &choice);
                switch (choice) {
                case 1: // Ввод данных вручную
                    collection_date();
                    break;
                case 2: { // Автоматическая генерация данных
                    Record* records = 0;
                    int count = 0;
                    generate_random_data(&records, &count);
                    free(records); // Освобождение памяти после использования
                    break;
                }
                case 3: // Возврат в главное меню
                    monitoring = 0;
                    break;
                default:
                    printf("Неверный выбор. Попробуйте еще раз.\n");
                }
            }
            break;
        case 2: // Отключение режима мониторинга
            monitoring = 0;
            printf("Режим мониторинга отключен.\n");
            break;
        case 3: // Анализ данных из файла
            analyz_data();
            break;
        case 4: // Выход из программы
            exit(0);
        default:
            printf("Неверный выбор. Попробуйте еще раз.\n");
        }
    }
    return 0;
}

int collection_date() {
    Record* records = malloc(sizeof(Record) * MAX_RECORD_COUNT); // Выделение памяти под массив записей
    if (!records) {
        printf("Ошибка выделения памяти.\n");
        return;
    }

    int count = 0;
    handle_user_input(records, &count); // Обработка ввода данных от пользователя
    data("data.txt", records, count); // Запись собранных данных в файл
    free(records); // Освобождение памяти после использования
}

int handle_user_input(Record* records, int* count) {
    while (*count < MAX_RECORD_COUNT) { // Ограничение на количество вводимых записей
        char input[MAX_RECORD_COUNT]; //Буфер для ввода данных пользователем
        printf("Введите уровень жидкости в формате '[значение] м, [индекс резервуара]' (или 'exit' для выхода): ");
        scanf(" %[^\n]s", input); // пробелы

        if (strcmp(input, "exit") == 0) { // Проверка команды выхода из ввода
            printf("Выход из режима мониторинга.\n");
            break; // Выход из цикла ввода
        }

        Record record;
        if (validate_input(input, &record)) { // Проверка корректности ввода
            records[*count] = record; // Сохранение записи
            (*count)++;
            check_monotonicity(records, *count); // Проверка монотонности уровней жидкости после добавления новой записи
        }
        else {
            printf("Ошибка формата ввода. Попробуйте еще раз.\n");
        }
    }
}

int validate_input(const char* input, Record* record) {
    char* token = strtok((char*)input, "м, ");
    if (token != NULL) {
        record->level = atof(token); // Преобразование первого значения в уровень жидкости (float)
        token = strtok(NULL, "м, ");
        if (token != NULL) {
            record->index = atoi(token); // Преобразование второго значения в индекс резервуара (int)
            if (record->level >= 0 && record->level <= POROG) {
                return 1; // Успешная валидация
            }
        }
    }
    return 0; // Ошибка валидации
}

int check_monotonicity(Record* records, int count) { 
    // Проверка, что есть как минимум 2 записи и что индексы резервуаров совпадают
    if (count > 1 && records[count - 1].index == records[count - 2].index) { 
        // Проверка условий монотонности уровня жидкости
        if ((records[count - 1].level < records[count - 2].level - 1) || 
            // Уровень жидкости последней записи не должен отличаться от уровня предыдущей записи более чем на 1 единицу
            (records[count - 1].level > records[count - 2].level + 1)) { 
            printf("Авария: нарушена монотонность уровня жидкости для резервуара %d!\n", records[count - 1].index);
        }
    }
    return 0;
}

int generate_random_data(Record** records, int* count) {
    srand(time(NULL)); // Инициализация генератора случайных чисел
    *count = MAX_RECORD_COUNT; // Установка количества записей

    *records = malloc(sizeof(Record) * (*count));
    if (*records == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }

    float previous_level[MAX_TANKS] = { 0 }; // Массив для хранения предыдущих уровней по резервуарам
    for (int i = 0; i < MAX_TANKS; i++) {
        previous_level[i] = rand() % (POROG + 1); // Инициализация случайных уровней
    }

    for (int i = 0; i < *count; i++) {
        float new_level;
        int tank_index = rand() % MAX_TANKS + 1; // Генерация индекса резервуара от 1 до MAX_TANKS

        do {
            new_level = previous_level[tank_index - 1] + ((rand() % 3) - 1); // Генерация нового уровня
            new_level = new_level < 0 ? 0 : new_level > POROG ? POROG : new_level; // Ограничение по границам
        } while ((new_level < previous_level[tank_index - 1] - 1) ||
            (new_level > previous_level[tank_index - 1] + 1));

        (*records)[i].level = new_level;
        (*records)[i].index = tank_index;

        // Запись данных в соответствующий файл резервуара
        char filename[MAX_FILENAME_SIZE];
        sprintf(filename, "tank_%d.txt", tank_index); // Формирование имени файла
        FILE* tank_file = fopen(filename, "a"); // Открытие файла для добавления данных
        if (tank_file != NULL) {
            write_record_to_file(tank_file, (*records)[i]);
            fclose(tank_file);
        }
        else {
            printf("Ошибка открытия файла %s для записи.\n", filename);
        }
        previous_level[tank_index - 1] = new_level; // Обновление предыдущего уровня для резервуара
    }

    printf("Данные записаны в файлы резервуаров.\n");
    sort_records(*records, *count); // Сортировка записей всех резервуарам
    return 0;
}

int write_record_to_file(FILE* file, Record record) {
    fprintf(file, "%.2f %d\n", record.level, record.index);
}

int sort_records(Record* records, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if ((records[i].index > records[j].index) ||
                (records[i].index == records[j].index && records[i].level > records[j].level)) {
                Record temp = records[i];
                records[i] = records[j];
                records[j] = temp;
            }
        }
    }
    data("data.txt", records, count); // Запись всех данных в общий файл
}

int data(const char* filename, Record* records, int count) {
    FILE* file = fopen(filename, "a");
    if (file == 0) {
        printf("Ошибка открытия файла для записи.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        write_record_to_file(file, records[i]);
    }

    fclose(file);
    printf("Данные записаны в файл %s.\n", filename);
    return 0;
}

int analyz_data() {
    int tank_index;

    // Запрос индекса резервуара у пользователя
    printf("Введите номер резервуара для анализа (от 1 до %d): ", MAX_TANKS);
    scanf("%d", &tank_index);

    if (tank_index < 1 || tank_index > MAX_TANKS) { // Проверка на допустимость введенного индекса резервуар
        printf("Ошибка: неверный индекс резервуара.\n");
        return;
    }

    char filename[MAX_FILENAME_SIZE];
    sprintf(filename, "tank_%d.txt", tank_index); // Формирование имени файла на основе индекса резервуара

    FILE* file = fopen(filename, "r"); // Открытие файла для чтения
    if (file == 0) {
        printf("Ошибка открытия файла %s для анализа.\n", filename);
        return;
    }

    float total = 0; // Переменная для хранения общего уровня
    float max_level = -1; // Инициализация минимального уровня
    float min_level = POROG + 1; // Инициализация максимального уровня на большее значение чем POROG
    int count = 0; // Счетчик записей в файле

    while (!feof(file)) { // Чтение данных из файла, пока не достигнут конец файла
        Record record;
        fscanf(file, "%f %d", &record.level, &record.index); // Чтение уровня и индекса
        total += record.level; // Суммирование уровня

        if (record.level > max_level) {
            max_level = record.level; // Обновление максимального уровня
        }

        if (record.level < min_level) {
            min_level = record.level; // Обновление минимального уровня
        }

        count++;
    }

    fclose(file); // Закрытие файла

    print_average_level(total, count); // Печать среднего уровня жидкости

    if (count > 0) {
        print_max_min_levels(max_level, min_level); // Печать максимального и минимального уровней жидкости
    }
    else {
        printf("Нет записанных данных для анализа.\n");
    }
}

int print_average_level(float total, int count) {
    if (count > 0) { // Проверка на допустимость количества записей
        float average_level = total / count; // Вычисление среднего уровня
        printf("Средний уровень жидкости: %.2f м\n", average_level);
    }
    else {
        printf("Нет записанных данных для анализа.\n");
    }
}

int print_max_min_levels(float max, float min) {
    if (max >= 0 && min <= POROG) { // Проверка на допустимые значения уровней жидкости
        printf("Максимальный уровень жидкости: %.2f м\n", max); // Печать максимального уровня
        printf("Минимальный уровень жидкости: %.2f м\n", min); // Печать минимального уровня
    }
    else {
        printf("Нет записанных данных для анализа.\n");
    }
}