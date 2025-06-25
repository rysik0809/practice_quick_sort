#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>   
#include <time.h>    

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

int main() {
    const int n = 20;
    int arr[n];
    int choice;
    int min, max;       
    int temp;          

    srand(time(NULL));

    printf("Выберите способ заполнения массива:\n");
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
    quickSort(arr, 0, n - 1);

    printf("\nОтсортированный массив:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}