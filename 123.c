#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <omp.h>

#define CHUNK_SIZE 100000
#define DEFAULT_RUNS 100

int main() {
    setlocale(LC_ALL, "Russian");

    // Ввод параметров
    size_t size;
    int num_threads;
    int runs;

    printf("Введите количество элементов массива: ");
    if (scanf_s("%zu", &size) != 1 || size <= 0) {
        printf("Ошибка: введите положительное число!\n");
        return 1;
    }

    printf("Введите количество потоков (0 для автоматического): ");
    if (scanf_s("%d", &num_threads) != 1) {
        printf("Ошибка ввода!\n");
        return 1;
    }

    printf("Введите количество запусков (по умолчанию %d): ", DEFAULT_RUNS);
    if (scanf_s("%d", &runs) != 1 || runs <= 0) {
        runs = DEFAULT_RUNS;
    }

    // Установка количества потоков
    if (num_threads > 0) {
        omp_set_num_threads(num_threads);
    }

    // Переменные для статистики
    double total_fill_time = 0.0;
    double total_calc_time = 0.0;
    double min_time = 1e9;
    double max_time = 0.0;
    long long checksum = 0;

    printf("\nЗапускаю тестирование (%d прогонов)...\n", runs);

    for (int run = 0; run < runs; run++) {
        double run_start = omp_get_wtime();

        // Заполнение массива
        double fill_start = omp_get_wtime();
        int* array = (int*)malloc(size * sizeof(int));
        if (array == NULL) {
            printf("Ошибка выделения памяти!\n");
            return 1;
        }

        srand(time(NULL) + run);
#pragma omp parallel for
        for (size_t i = 0; i < size; i++) {
            array[i] = rand() % 100;
        }
        double fill_end = omp_get_wtime();

        // Вычисление суммы
        double calc_start = omp_get_wtime();
        long long sum = 0;
#pragma omp parallel for reduction(+:sum) schedule(dynamic, CHUNK_SIZE)
        for (size_t i = 0; i < size; i++) {
            sum += array[i];
        }
        double calc_end = omp_get_wtime();
        double run_end = omp_get_wtime();

        // Сбор статистики
        double fill_time = fill_end - fill_start;
        double calc_time = calc_end - calc_start;
        double total_time = run_end - run_start;

        total_fill_time += fill_time;
        total_calc_time += calc_time;
        checksum += sum;

        if (total_time < min_time) min_time = total_time;
        if (total_time > max_time) max_time = total_time;

        if (run == 0) {
            printf("\n\033[1mПараметры теста:\033[0m\n");
            printf("Элементов: %zu\n", size);
            printf("Потоков: %d\n", omp_get_max_threads());
        }

        free(array);
    }

    // Вывод результатов
    printf("\n\033[1mРезультаты после %d запусков:\033[0m\n", runs);
    printf("Среднее время заполнения: %.4f сек\n", total_fill_time / runs);
    printf("Среднее время вычислений: %.4f сек\n", total_calc_time / runs);
    printf("Среднее общее время: %.4f сек\n", (total_fill_time + total_calc_time) / runs);
    printf("Использовано потоков: %d\n", omp_get_max_threads());

    return 0;
}