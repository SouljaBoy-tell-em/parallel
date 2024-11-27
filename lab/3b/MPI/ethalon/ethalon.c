#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../general.h"

double a[ISIZE][JSIZE];

void record(char *fileName, double a[ISIZE][JSIZE]) {
    FILE *ff = fopen(fileName,"w");

    for(int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            fprintf(ff, "%f ", a[i][j]);
        }
        fprintf(ff, "\n");
    }

    fclose(ff);
}

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
    record("ethalon.txt", a);

    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = 10 * i + j;
        }
    }
}