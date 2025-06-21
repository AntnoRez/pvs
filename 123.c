#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <locale.h>

#define MAX_SIZE 100000000
#define THRESHOLD 10000

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quick_sort_sequential(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_sequential(arr, low, pi - 1);
        quick_sort_sequential(arr, pi + 1, high);
    }
}

void quick_sort_parallel(int arr[], int low, int high, int threads) {
    if (low < high) {
        int pi = partition(arr, low, high);

        if (high - low > THRESHOLD && threads > 1) {
#pragma omp task shared(arr)
            quick_sort_parallel(arr, low, pi - 1, threads / 2);

#pragma omp task shared(arr)
            quick_sort_parallel(arr, pi + 1, high, threads / 2);

#pragma omp taskwait
        }
        else {
            quick_sort_sequential(arr, low, pi - 1);
            quick_sort_sequential(arr, pi + 1, high);
        }
    }
}

void fill_and_shuffle(int arr[], int size, int threads) {
#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < size; i++) {
        arr[i] = i + 1;
    }

    srand(time(NULL));
#pragma omp parallel for num_threads(threads)
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

int is_sorted(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    setlocale(LC_ALL, "Russian");

    // ���� ����������
    int size, threads, runs;
    printf("������� ���������� ��������� ��� ���������� (1 - %d): ", MAX_SIZE);
    if (scanf_s("%d", &size) != 1 || size <= 0 || size > MAX_SIZE) {
        printf("������������ ���� ������� �������!\n");
        return 1;
    }

    printf("������� ���������� ������� (1, 2, 4, 8, ...): ");
    if (scanf_s("%d", &threads) != 1 || threads <= 0) {
        printf("������������ ���� ���������� �������!\n");
        return 1;
    }

    printf("������� ���������� �������� ��� ������������: ");
    if (scanf_s("%d", &runs) != 1 || runs <= 0) {
        printf("������������ �������� �� ���������: 5\n");
        runs = 5;
    }

    // ��������� ������
    int* array = (int*)malloc(size * sizeof(int));
    if (array == NULL) {
        printf("������ ��������� ������ ��� %d ���������!\n", size);
        return 1;
    }

    omp_set_num_threads(threads);
    printf("\n������� ���������� (%d �������)\n", threads);
    printf("������ �������: %d ���������\n", size);
    printf("���������� ��������: %d\n", runs);

    // ���������� ��� ����������
    double total_fill_time = 0.0;
    double total_sort_time = 0.0;
    double min_time = 1e9;
    double max_time = 0.0;
    int success_runs = 0;

    for (int run = 1; run <= runs; run++) {
        // ���������� � �������������
        double start = omp_get_wtime();
        fill_and_shuffle(array, size, threads);
        double fill_time = omp_get_wtime() - start;
        total_fill_time += fill_time;

        // ����������
        start = omp_get_wtime();
#pragma omp parallel num_threads(threads)
        {
#pragma omp single
            quick_sort_parallel(array, 0, size - 1, threads);
        }
        double sort_time = omp_get_wtime() - start;
        total_sort_time += sort_time;

        double total_time = fill_time + sort_time;
        if (total_time < min_time) min_time = total_time;
        if (total_time > max_time) max_time = total_time;

        // �������� ����������
        if (is_sorted(array, size)) {
            success_runs++;
        }
    }

    // ����� ����������
    printf("\n�������� ����������:\n", success_runs, runs);
    printf("========================================\n");
    printf("������� ����� ����������:  %.4f ���\n", total_fill_time / runs);
    printf("������� ����� ����������:  %.4f ���\n", total_sort_time / runs);
    printf("������� ����� �����:       %.4f ���\n", (total_fill_time + total_sort_time) / runs);

    free(array);
    return 0;
}
