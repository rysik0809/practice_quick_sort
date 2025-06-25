#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>   
#include <time.h>    
#include <string.h>

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

int main() {
    const int n = 20;
    int arr[n];
    int choice;
    int min, max;
    int temp;
    char input_filename[100];  
    char output_filename[100]; 

    srand(time(NULL));

    printf("Введите название файла для исходных данных: ");
    scanf("%99s", input_filename);
    ensure_txt_extension(input_filename);

    printf("Введите название файла для результатов сортировки: ");
    scanf("%99s", output_filename);
    ensure_txt_extension(output_filename);

    printf("\nВыберите способ заполнения массива:\n");
    printf("1 - Ввести 20 чисел вручную\n");
    printf("2 - Автоматическое заполнение случайными числами\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nВведите 20 целых чисел:\n");
        for (int i = 0; i < n; i++) {
            printf("Элемент %d: ", i + 1);
            scanf("%d", &arr[i]);
        }
    }
    else if (choice == 2) {
        printf("\nВведите границы диапазона (min max): ");
        scanf("%d %d", &min, &max);

        if (min > max) {
            temp = min;
            min = max;
            max = temp;
            printf("Границы были автоматически поменяны местами.\n");
        }

        for (int i = 0; i < n; i++) {
            arr[i] = min + rand() % (max - min + 1);
        }

        printf("\nСгенерированный массив:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    else {
        printf("Ошибка! Неверный выбор.\n");
        return 1;
    }

    FILE* input_file = fopen(input_filename, "w");
    if (input_file == NULL) {
        printf("Ошибка создания файла для исходных данных!\n");
        return 1;
    }

    fprintf(input_file, "Исходные данные:\n");
    for (int i = 0; i < n; i++) {
        fprintf(input_file, "%d ", arr[i]);
    }
    fprintf(input_file, "\n");
    fclose(input_file);
    printf("\nИсходные данные сохранены в файл: %s\n", input_filename);

    quickSort(arr, 0, n - 1);

    FILE* output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Ошибка создания файла для результатов!\n");
        return 1;
    }

    fprintf(output_file, "Отсортированный массив:\n");
    for (int i = 0; i < n; i++) {
        fprintf(output_file, "%d ", arr[i]);
    }
    fprintf(output_file, "\n");
    fclose(output_file);
    printf("Результаты сортировки сохранены в файл: %s\n", output_filename);

    printf("\nОтсортированный массив:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}