#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <float.h>

void fill_random_values(double* dataset, int length) {
#pragma omp parallel for
    for (int idx = 0; idx < length; idx++) {
        dataset[idx] = ((double)rand() / (double)RAND_MAX) * 100.0;
    }
}

void process_arrays(double* input1, double* input2,
    double* sum_output, double* diff_output,
    double* product_output, double* quotient_output,
    int length) {
#pragma omp parallel for
    for (int pos = 0; pos < length; pos++) {
        sum_output[pos] = input1[pos] + input2[pos];
        diff_output[pos] = input1[pos] - input2[pos];
        product_output[pos] = input1[pos] * input2[pos];
        quotient_output[pos] = input1[pos] / input2[pos];
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <thread_count>\n", argv[0]);
        return 1;
    }

    int thread_count = atoi(argv[1]);
    omp_set_num_threads(thread_count);

    int data_length;
    printf("Enter number of elements: ");
    scanf_s("%d", &data_length);

    double* primary = (double*)malloc(data_length * sizeof(double));
    double* secondary = (double*)malloc(data_length * sizeof(double));
    double* summation = (double*)malloc(data_length * sizeof(double));
    double* difference = (double*)malloc(data_length * sizeof(double));
    double* multiplication = (double*)malloc(data_length * sizeof(double));
    double* division = (double*)malloc(data_length * sizeof(double));

    srand((unsigned)time(NULL));

    double total_time = 0.0;
    double min_time = DBL_MAX;
    double max_time = 0.0;

    for (int iter = 0; iter < 100; iter++) {
        fill_random_values(primary, data_length);
        fill_random_values(secondary, data_length);

        double start = omp_get_wtime();
        process_arrays(primary, secondary,
            summation, difference,
            multiplication, division,
            data_length);
        double end = omp_get_wtime();

        double elapsed = end - start;
        total_time += elapsed;
        if (elapsed < min_time) min_time = elapsed;
        if (elapsed > max_time) max_time = elapsed;
    }

    printf("\nAfter 100 iterations with %d elements (%d threads):\n", data_length, thread_count);
    printf("Average time: %.4f s\n", total_time / 100);
    printf("Minimum time: %.4f s\n", min_time);
    printf("Maximum time: %.4f s\n", max_time);

    free(primary); free(secondary);
    free(summation); free(difference);
    free(multiplication); free(division);

    return 0;
}