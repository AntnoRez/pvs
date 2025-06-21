#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

int main() {
    setlocale(LC_ALL, "Russian");

    // ���� ���������� ��������� �������
    size_t SIZE;
    printf("������� ���������� ��������� �������: ");
    scanf_s("%zu", &SIZE);

    // ���� ���������� ��������
    int runs;
    printf("������� ���������� �������� ��� ������������: ");
    scanf_s("%d", &runs);

    // �������� �� ������������ �����
    if (SIZE <= 0 || runs <= 0) {
        printf("������: ���������� ��������� � �������� ������ ���� �������������!\n");
        return 1;
    }

    // ���������� ��� ���������� �������
    double total_fill_time = 0.0;
    double total_calc_time = 0.0;
    double total_time = 0.0;

    for (int run = 0; run < runs; run++) {
        clock_t total_start = clock();

        clock_t fill_start = clock();
        int* array = (int*)malloc(SIZE * sizeof(int));
        if (array == NULL) {
            printf("������ ��������� ������!\n");
            return 1;
        }

        // ������������� ���������� ��������� �����
        srand(time(NULL) + run);  // ��������� run ��� ������ seed ��� ������ �������
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

        if (run == 0) {  // ������� ���������� ������ ��� ������� �������
            printf("\033[1m���������������� ������\033[0m\n");
            printf("���������� ���������: %zu\n", SIZE);
        }

        free(array);
    }

    // ��������� ������� �����
    double avg_fill_time = total_fill_time / runs;
    double avg_calc_time = total_calc_time / runs;
    double avg_total_time = total_time / runs;

    printf("\n���������� ����� %d ��������:\n", runs);
    printf("������� ����� ���������� �������: %.4f ���\n", avg_fill_time);
    printf("������� ����� ����������: %.4f ���\n", avg_calc_time);
    printf("������� ����� �����: %.4f ���\n", avg_total_time);

    return 0;
}