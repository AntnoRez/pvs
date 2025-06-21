#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

int main() {
    setlocale(LC_ALL, "Russian");

    // Ââîä êîëè÷åñòâà ýëåìåíòîâ ìàññèâà
    size_t SIZE;
    printf("Ââåäèòå êîëè÷åñòâî ýëåìåíòîâ ìàññèâà: ");
    scanf_s("%zu", &SIZE);

    // Ââîä êîëè÷åñòâà çàïóñêîâ
    int runs;
    printf("Ââåäèòå êîëè÷åñòâî çàïóñêîâ äëÿ òåñòèðîâàíèÿ: ");
    scanf_s("%d", &runs);

    // Ïðîâåðêà íà êîððåêòíîñòü ââîäà
    if (SIZE <= 0 || runs <= 0) {
        printf("Îøèáêà: êîëè÷åñòâî ýëåìåíòîâ è çàïóñêîâ äîëæíî áûòü ïîëîæèòåëüíûì!\n");
        return 1;
    }

    // Ïåðåìåííûå äëÿ íàêîïëåíèÿ âðåìåíè
    double total_fill_time = 0.0;
    double total_calc_time = 0.0;
    double total_time = 0.0;

    for (int run = 0; run < runs; run++) {
        clock_t total_start = clock();

        clock_t fill_start = clock();
        int* array = (int*)malloc(SIZE * sizeof(int));
        if (array == NULL) {
            printf("Îøèáêà âûäåëåíèÿ ïàìÿòè!\n");
            return 1;
        }

        // Èíèöèàëèçàöèÿ ãåíåðàòîðà ñëó÷àéíûõ ÷èñåë
        srand(time(NULL) + run);  // Äîáàâëÿåì run äëÿ ðàçíûõ seed ïðè êàæäîì çàïóñêå
        for (size_t i = 0; i < SIZE; i++) {
            array[i] = rand() % 100;
        }
        clock_t fill_end = clock();

        clock_t calc_start = clock();
        long long sum = 0;
        for (size_t i = 0; i < SIZE; i++) {
            sum += array[i];
        }
        clock_t calc_end = clock();

        clock_t total_end = clock();

        double fill_time = (double)(fill_end - fill_start) / CLOCKS_PER_SEC;
        double calc_time = (double)(calc_end - calc_start) / CLOCKS_PER_SEC;
        double current_total_time = (double)(total_end - total_start) / CLOCKS_PER_SEC;

        total_fill_time += fill_time;
        total_calc_time += calc_time;
        total_time += current_total_time;

        if (run == 0) {  // Âûâîäèì èíôîðìàöèþ òîëüêî äëÿ ïåðâîãî çàïóñêà
            printf("\033[1mÏîñëåäîâàòåëüíàÿ âåðñèÿ\033[0m\n");
            printf("Êîëè÷åñòâî ýëåìåíòîâ: %zu\n", SIZE);
        }

        free(array);
    }

    // Âû÷èñëÿåì ñðåäíåå âðåìÿ
    double avg_fill_time = total_fill_time / runs;
    double avg_calc_time = total_calc_time / runs;
    double avg_total_time = total_time / runs;

    printf("\nÐåçóëüòàòû ïîñëå %d çàïóñêîâ:\n", runs);
    printf("Ñðåäíåå âðåìÿ çàïîëíåíèÿ ìàññèâà: %.4f ñåê\n", avg_fill_time);
    printf("Ñðåäíåå âðåìÿ âû÷èñëåíèé: %.4f ñåê\n", avg_calc_time);
    printf("Ñðåäíåå îáùåå âðåìÿ: %.4f ñåê\n", avg_total_time);

    return 0;
}
