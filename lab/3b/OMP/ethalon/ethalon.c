    #include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "../general.h"

double a[ISIZE][JSIZE];

int main(int argc, char **argv) {
    struct timeval start, end;

    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = 10 * i + j;
        }
    }

    gettimeofday(&start, NULL);
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = sin(2 * a[i][j]);
        }
    }
    gettimeofday(&end, NULL);

    printf("ethalon time: %.4lf seconds\n", (double)(end.tv_usec - start.tv_usec) / 1000000);
    record(EHTALON, a);

    init(a);

    gettimeofday(&start, NULL);
#pragma omp parallel for collapse(2)
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = sin(2 * a[i][j]);
        }
    }
    gettimeofday(&end, NULL);

    printf("ethalon time: %.4lf seconds\n", (double)(end.tv_usec - start.tv_usec) / 1000000);
    record(EHTALON_PAR, a);

    // clang -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp ethalon.c -o ethalon && ./ethalon
    // diff ethalon.txt ethalon_par.txt
}