#include <stdio.h>   // Для printf и scanf_s
#include <stdlib.h>  // Для malloc, free и rand
#include <time.h>    // Для clock и time
#include <locale.h>  // Для setlocale

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int* arr, int low, int high) {
    if (high - low < 100) {
        for (int i = low + 1; i <= high; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= low && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }

    int pi = partition(arr, low, high);

    if (pi - low < high - pi) {
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
    else {
        quickSort(arr, pi + 1, high);
        quickSort(arr, low, pi - 1);
    }
}

void shuffleArray(int* arr, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    int SIZE, runs;
    printf("Введите количество элементов для сортировки (1 - 100000000): ");
    if (scanf_s("%d", &SIZE) != 1 || SIZE <= 0 || SIZE > 100000000) {
        printf("Некорректный ввод! Должно быть число от 1 до 100000000.\n");
        return 1;
    }

    printf("Введите количество запусков для тестирования: ");
    if (scanf_s("%d", &runs) != 1 || runs <= 0) {
        printf("Используется значение по умолчанию: 10\n");
        runs = 10;
    }

    double total_fill_time = 0.0;
    double total_shuffle_time = 0.0;
    double total_sort_time = 0.0;
    double min_time = 1e9;
    double max_time = 0.0;
    int* arr = NULL;

    for (int run = 0; run < runs; run++) {
        clock_t program_start = clock();

        if (arr == NULL) {
            arr = (int*)malloc(SIZE * sizeof(int));
            if (arr == NULL) {
                printf("Ошибка выделения памяти для %d элементов!\n", SIZE);
                return 1;
            }
        }

        clock_t fill_start = clock();
        for (int i = 0; i < SIZE; i++) {
            arr[i] = i + 1;
        }
        clock_t fill_end = clock();
        double fill_time = (double)(fill_end - fill_start) / CLOCKS_PER_SEC;
        total_fill_time += fill_time;

        clock_t shuffle_start = clock();
        srand(time(NULL) + run);
        shuffleArray(arr, SIZE);
        clock_t shuffle_end = clock();
        double shuffle_time = (double)(shuffle_end - shuffle_start) / CLOCKS_PER_SEC;
        total_shuffle_time += shuffle_time;

        clock_t sort_start = clock();
        quickSort(arr, 0, SIZE - 1);
        clock_t sort_end = clock();
        double sort_time = (double)(sort_end - sort_start) / CLOCKS_PER_SEC;
        total_sort_time += sort_time;

        clock_t program_end = clock();
        double total_time = (double)(program_end - program_start) / CLOCKS_PER_SEC;

        if (total_time < min_time) min_time = total_time;
        if (total_time > max_time) max_time = total_time;
    }

    printf("\n\nРезультаты для %d элементов (%d запусков):\n", SIZE, runs);
    printf("================================\n");
    printf("Среднее время заполнения:  %.4f сек\n", total_fill_time / runs);
    printf("Среднее время сортировки:  %.4f сек\n", total_sort_time / runs);
    printf("Среднее общее время:       %.4f сек\n",
        (total_fill_time + total_shuffle_time + total_sort_time) / runs);

    free(arr);
    return 0;
}
