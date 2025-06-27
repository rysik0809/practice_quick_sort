#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <limits.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

// Улучшенная функция безопасного ввода целого числа
int safe_input_int(int* value, const char* prompt) {
    char buffer[100];
    char* endptr;
    long int long_value;

    while (1) {
        if (prompt != NULL) printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0; // Ошибка ввода
        }

        // Удаляем символ новой строки
        buffer[strcspn(buffer, "\n")] = '\0';

        // Пропускаем ведущие пробелы
        char* start = buffer;
        while (*start == ' ') start++;

        if (*start == '\0') {
            printf("Ошибка: Пустой ввод. Пожалуйста, введите целое число: ");
            continue;
        }

        errno = 0;
        long_value = strtol(start, &endptr, 10);

        // Проверяем ошибки преобразования
        if (errno == ERANGE || long_value < INT_MIN || long_value > INT_MAX) {
            printf("Ошибка: Число вне допустимого диапазона. Пожалуйста, введите число от %d до %d: ",
                INT_MIN, INT_MAX);
            continue;
        }

        // Проверяем, что весь ввод был обработан
        while (*endptr == ' ') endptr++;
        if (*endptr != '\0') {
            printf("Ошибка: Некорректный ввод. Обнаружены нечисловые символы: '%s'. Пожалуйста, введите целое число: ",
                endptr);
            continue;
        }

        *value = (int)long_value;
        return 1;
    }
}

void printSeparator() {
    printf("\n==================================================\n");
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int p = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        while (arr[i] <= p && i <= high - 1) {
            i++;
        }

        while (arr[j] > p && j >= low + 1) {
            j--;
        }
        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[low], &arr[j]);
    return j;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void ensure_txt_extension(char* filename) {
    if (strstr(filename, ".txt") == NULL) {
        strcat(filename, ".txt");
    }
}

double get_current_time() {
#ifdef _WIN32
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#endif
}

int main() {
    setlocale(LC_ALL, "Russian");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int repeat = 1;

    while (repeat) {
        printSeparator();
        printf("\t\tПРОГРАММА ДЛЯ СОРТИРОВКИ МАССИВОВ\n");
        printSeparator();

        int size;
        int* arr = NULL;
        int choice;
        int min, max;
        int temp;
        char input_filename[100];
        char output_filename[100];
        double start_time, end_time, cpu_time_used;

        srand(time(NULL));

        // Ввод размера массива с проверкой
        while (!safe_input_int(&size, "\n> Введите количество элементов для сортировки: ")) {
            printf("Попробуйте еще раз: ");
        }

        while (size <= 0) {
            printf("Ошибка: Размер должен быть положительным числом. ");
            while (!safe_input_int(&size, "Повторите ввод: ")) {
                printf("Попробуйте еще раз: ");
            }
        }

        arr = (int*)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("\n> ОШИБКА: Не удалось выделить память!\n");
            return 1;
        }

        printf("\n> Введите имя файла для исходных данных: ");
        fgets(input_filename, sizeof(input_filename), stdin);
        input_filename[strcspn(input_filename, "\n")] = '\0';
        ensure_txt_extension(input_filename);

        printf("> Введите имя файла для результатов: ");
        fgets(output_filename, sizeof(output_filename), stdin);
        output_filename[strcspn(output_filename, "\n")] = '\0';
        ensure_txt_extension(output_filename);

        // Ввод выбора действия с проверкой
        printf("\n> Выберите способ заполнения массива:\n");
        printf("1. Ручной ввод\n");
        printf("2. Автоматическая генерация\n");
        printf("3. Выход из программы\n");

        while (!safe_input_int(&choice, "\nВаш выбор: ")) {
            printf("Попробуйте еще раз: ");
        }

        while (choice < 1 || choice > 3) {
            printf("Ошибка: Введите число от 1 до 3. ");
            while (!safe_input_int(&choice, "Повторите ввод: ")) {
                printf("Попробуйте еще раз: ");
            }
        }

        if (choice == 3) {
            printf("\nЗавершение программы...\n");
            free(arr);
            break;
        }

        if (choice == 1) {
            // Ручной ввод элементов с проверкой
            printf("\n> Введите %d целых чисел:\n", size);
            for (int i = 0; i < size; i++) {
                char prompt[50];
                sprintf(prompt, "Элемент %d: ", i + 1);
                while (!safe_input_int(&arr[i], prompt)) {
                    printf("Попробуйте еще раз: ");
                }
            }
        }
        else if (choice == 2) {
            // Ввод границ диапазона с проверкой
            while (!safe_input_int(&min, "\n> Введите минимальное значение: ")) {
                printf("Попробуйте еще раз: ");
            }

            while (!safe_input_int(&max, "> Введите максимальное значение: ")) {
                printf("Попробуйте еще раз: ");
            }

            if (min > max) {
                temp = min;
                min = max;
                max = temp;
                printf("\n> Границы скорректированы: %d %d\n", min, max);
            }

            for (int i = 0; i < size; i++) {
                arr[i] = min + rand() % (max - min + 1);
            }

            printf("\n> Сгенерированный массив (%d элементов):\n", size);
            for (int i = 0; i < size; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
        }

        FILE* input_file = fopen(input_filename, "w");
        if (input_file == NULL) {
            printf("\n> ОШИБКА: Не удалось создать файл исходных данных!\n");
            free(arr);
            return 1;
        }

        fprintf(input_file, "Исходные данные (%d элементов):\n", size);
        for (int i = 0; i < size; i++) {
            fprintf(input_file, "%d ", arr[i]);
        }
        fprintf(input_file, "\n");
        fclose(input_file);
        printf("\n> Исходные данные сохранены в файл: %s\n", input_filename);

        int* original_arr = (int*)malloc(size * sizeof(int));
        if (original_arr == NULL) {
            printf("\n> ОШИБКА: Не удалось выделить память!\n");
            free(arr);
            return 1;
        }
        memcpy(original_arr, arr, size * sizeof(int));

        start_time = get_current_time();
        quickSort(arr, 0, size - 1);
        end_time = get_current_time();
        cpu_time_used = end_time - start_time;

        FILE* output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            printf("\n> ОШИБКА: Не удалось создать файл результатов!\n");
            free(arr);
            free(original_arr);
            return 1;
        }

        fprintf(output_file, "Отсортированный массив (%d элементов):\n", size);
        for (int i = 0; i < size; i++) {
            fprintf(output_file, "%d ", arr[i]);
        }
        fprintf(output_file, "\n\nВремя сортировки: %.9f секунд\n", cpu_time_used);
        fclose(output_file);
        printf("> Результаты сохранены в файл: %s\n", output_filename);

        printf("\n> Отсортированный массив (%d элементов):\n", size);
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }

        printf("\n\n> Сортировка выполнена за %.9f секунд\n", cpu_time_used);
        printf("> Отсортировано элементов: %d\n", size);

        free(arr);
        free(original_arr);

        // Проверка ввода для повторения программы
        printSeparator();
        printf("\n> Продолжить?\n");
        printf("1. Новая сортировка\n");
        printf("0. Выход\n");

        while (!safe_input_int(&repeat, "\nВаш выбор: ")) {
            printf("Попробуйте еще раз: ");
        }

        while (repeat != 0 && repeat != 1) {
            printf("Ошибка: Введите 0 или 1. ");
            while (!safe_input_int(&repeat, "Повторите ввод: ")) {
                printf("Попробуйте еще раз: ");
            }
        }
    }

    printf("\nПрограмма завершена. До свидания!\n");
    printSeparator();
    return 0;
}