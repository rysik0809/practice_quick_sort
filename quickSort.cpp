#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>   
#include <time.h>    
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

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
    int repeat = 1;

    while (repeat) {
        printSeparator();
        printf("\t\tARRAY SORTING PROGRAM\n");
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

        printf("\n> Enter the number of elements to sort: ");
        while (scanf("%d", &size) != 1 || size <= 0) {
            printf("> ERROR: Invalid size! Please enter a positive integer: ");
            while (getchar() != '\n'); 
        }

        arr = (int*)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("\n> ERROR: Memory allocation failed!\n");
            return 1;
        }

        printf("\n> Enter source data filename: ");
        scanf("%99s", input_filename);
        ensure_txt_extension(input_filename);

        printf("> Enter result filename: ");
        scanf("%99s", output_filename);
        ensure_txt_extension(output_filename);

        printf("\n> Choose input method:\n");
        printf("1. Manual input\n");
        printf("2. Random generation\n");
        printf("3. Exit program\n");
        printf("\nYour choice: ");
        scanf("%d", &choice);

        if (choice == 3) {
            printf("\nExiting program...\n");
            free(arr); 
            break;
        }

        if (choice == 1) {
            printf("\n> Enter %d integers:\n", size);
            for (int i = 0; i < size; i++) {
                printf("Element %d: ", i + 1);
                scanf("%d", &arr[i]);
            }
        }
        else if (choice == 2) {
            printf("\n> Enter range (min max): ");
            scanf("%d %d", &min, &max);

            if (min > max) {
                temp = min;
                min = max;
                max = temp;
                printf("\n> Range corrected to: %d %d\n", min, max);
            }

            for (int i = 0; i < size; i++) {
                arr[i] = min + rand() % (max - min + 1);
            }

            printf("\n> Generated array (%d elements):\n", size);
            for (int i = 0; i < size; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
        }
        else {
            printf("\n> ERROR: Invalid choice. Please try again.\n");
            free(arr);
            continue;
        }

        FILE* input_file = fopen(input_filename, "w");
        if (input_file == NULL) {
            printf("\n> ERROR: Cannot create source file!\n");
            free(arr);
            return 1;
        }

        fprintf(input_file, "Initial data (%d elements):\n", size);
        for (int i = 0; i < size; i++) {
            fprintf(input_file, "%d ", arr[i]);
        }
        fprintf(input_file, "\n");
        fclose(input_file);
        printf("\n> Source data saved to: %s\n", input_filename);

        int* original_arr = (int*)malloc(size * sizeof(int));
        if (original_arr == NULL) {
            printf("\n> ERROR: Memory allocation failed!\n");
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
            printf("\n> ERROR: Cannot create result file!\n");
            free(arr);
            free(original_arr);
            return 1;
        }

        fprintf(output_file, "Sorted array (%d elements):\n", size);
        for (int i = 0; i < size; i++) {
            fprintf(output_file, "%d ", arr[i]);
        }
        fprintf(output_file, "\n\nSorting time: %.9f seconds\n", cpu_time_used);
        fclose(output_file);
        printf("> Results saved to: %s\n", output_filename);

        printf("\n> Sorted array (%d elements):\n", size);
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }

        printf("\n\n> Sorting completed in %.9f seconds\n", cpu_time_used);
        printf("> Elements sorted: %d\n", size);

        free(arr);
        free(original_arr);

        printSeparator();
        printf("\n> Continue?\n");
        printf("1. New sorting\n");
        printf("0. Exit\n");
        printf("\nYour choice: ");
        scanf("%d", &repeat);
    }

    printf("\nProgram completed. Goodbye!\n");
    printSeparator();
    return 0;
}