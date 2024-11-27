#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include <stdlib.h>

#include "general.h"

double a[ISIZE][JSIZE], b[ISIZE][JSIZE];

void calculate(int num_threads, double * ethalon_time, double * par_time) {
    struct timeval start, end;
    double delta = 0;

    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = 10 * i +j;
            b[i][j] = 0;
        }
    }

    gettimeofday(&start, NULL);
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = sin(0.01 * a[i][j]);
        }
    }

    for (int i = 0; i < ISIZE - 1; i++){
        for (int j = 3; j < JSIZE; j++){
            b[i][j] = a[i + 1][j - 3] * 2;
        }
    }
    gettimeofday(&end, NULL);

    delta = (double)(end.tv_usec - start.tv_usec) / 1000000;
    printf("ethalon time: %.4lf seconds\n", delta);
    *ethalon_time = delta;
    record(MAIN_3B, b);

    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = 10 * i + j;
            b[i][j] = 0;
        }
    }

    omp_set_num_threads(num_threads);
    gettimeofday(&start, NULL);

#pragma omp parallel for collapse(2)
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = sin(0.01 * a[i][j]);
        }
    }

#pragma omp parallel for collapse(2)
    for (int i = 0; i < ISIZE - 1; i++){
        for (int j = 3; j < JSIZE; j++){
            b[i][j] = a[i + 1][j - 3] * 2;
        }
    }

    gettimeofday(&end, NULL);

    delta = (double)(end.tv_usec - start.tv_usec) / 1000000;
    printf("par time: %.4lf seconds\n", delta);
    *par_time = delta;
    record(MAIN_3B_PAR, b);

    #pragma omp parallel
    {
        #pragma omp critical
        {
            if (omp_get_thread_num() == 0) {
                printf("THREADS: %d\n", omp_get_num_threads());
            }
        }
    }
}

int main(int argc, char **argv) {
    double ethalon_time = 0.0, par_time = 0.0;
    calculate(atoi(argv[1]), &ethalon_time, &par_time);

    writeData("ethalon_dataset_3b.txt", ethalon_time);
    writeData("par_dataset_3b.txt", par_time);

    return 0;
}