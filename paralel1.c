#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <omp.h>

#define CHUNK_SIZE 100000
#define DEFAULT_RUNS 100

int main() {
    setlocale(LC_ALL, "Russian");

    // Ââîä ïàðàìåòðîâ
    size_t size;
    int num_threads;
    int runs;

    printf("Ââåäèòå êîëè÷åñòâî ýëåìåíòîâ ìàññèâà: ");
    if (scanf_s("%zu", &size) != 1 || size <= 0) {
        printf("Îøèáêà: ââåäèòå ïîëîæèòåëüíîå ÷èñëî!\n");
        return 1;
    }

    printf("Ââåäèòå êîëè÷åñòâî ïîòîêîâ (0 äëÿ àâòîìàòè÷åñêîãî): ");
    if (scanf_s("%d", &num_threads) != 1) {
        printf("Îøèáêà ââîäà!\n");
        return 1;
    }

    printf("Ââåäèòå êîëè÷åñòâî çàïóñêîâ (ïî óìîë÷àíèþ %d): ", DEFAULT_RUNS);
    if (scanf_s("%d", &runs) != 1 || runs <= 0) {
        runs = DEFAULT_RUNS;
    }

    // Óñòàíîâêà êîëè÷åñòâà ïîòîêîâ
    if (num_threads > 0) {
        omp_set_num_threads(num_threads);
    }

    // Ïåðåìåííûå äëÿ ñòàòèñòèêè
    double total_fill_time = 0.0;
    double total_calc_time = 0.0;
    double min_time = 1e9;
    double max_time = 0.0;
    long long checksum = 0;

    printf("\nÇàïóñêàþ òåñòèðîâàíèå (%d ïðîãîíîâ)...\n", runs);

    for (int run = 0; run < runs; run++) {
        double run_start = omp_get_wtime();

        // Çàïîëíåíèå ìàññèâà
        double fill_start = omp_get_wtime();
        int* array = (int*)malloc(size * sizeof(int));
        if (array == NULL) {
            printf("Îøèáêà âûäåëåíèÿ ïàìÿòè!\n");
            return 1;
        }

        srand(time(NULL) + run);
#pragma omp parallel for
        for (size_t i = 0; i < size; i++) {
            array[i] = rand() % 100;
        }
        double fill_end = omp_get_wtime();

        // Âû÷èñëåíèå ñóììû
        double calc_start = omp_get_wtime();
        long long sum = 0;
#pragma omp parallel for reduction(+:sum) schedule(dynamic, CHUNK_SIZE)
        for (size_t i = 0; i < size; i++) {
            sum += array[i];
        }
        double calc_end = omp_get_wtime();
        double run_end = omp_get_wtime();

        // Ñáîð ñòàòèñòèêè
        double fill_time = fill_end - fill_start;
        double calc_time = calc_end - calc_start;
        double total_time = run_end - run_start;

        total_fill_time += fill_time;
        total_calc_time += calc_time;
        checksum += sum;

        if (total_time < min_time) min_time = total_time;
        if (total_time > max_time) max_time = total_time;

        if (run == 0) {
            printf("\n\033[1mÏàðàìåòðû òåñòà:\033[0m\n");
            printf("Ýëåìåíòîâ: %zu\n", size);
            printf("Ïîòîêîâ: %d\n", omp_get_max_threads());
        }

        free(array);
    }

    // Âûâîä ðåçóëüòàòîâ
    printf("\n\033[1mÐåçóëüòàòû ïîñëå %d çàïóñêîâ:\033[0m\n", runs);
    printf("Ñðåäíåå âðåìÿ çàïîëíåíèÿ: %.4f ñåê\n", total_fill_time / runs);
    printf("Ñðåäíåå âðåìÿ âû÷èñëåíèé: %.4f ñåê\n", total_calc_time / runs);
    printf("Ñðåäíåå îáùåå âðåìÿ: %.4f ñåê\n", (total_fill_time + total_calc_time) / runs);
    printf("Èñïîëüçîâàíî ïîòîêîâ: %d\n", omp_get_max_threads());

    return 0;
}
