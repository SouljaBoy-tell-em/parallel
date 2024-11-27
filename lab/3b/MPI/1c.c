#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "general.h"

int main(int argc, char **argv)
{
    struct timeval start, end;

    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10 * i + j;
        }
    }


    gettimeofday(&start, NULL);
    for (i=2; i<ISIZE; i++){
        for (j = 0; j < JSIZE-3; j++){
            a[i][j] = sin(5*a[i-2][j+3]);
        }
    }
    gettimeofday(&end, NULL);

    ff = fopen("1c.txt", "w");
    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);

    double delta = (double)(end.tv_usec - start.tv_usec) / 1000000;
    ff = fopen("ethalon_dataset_1c.txt", "w");
    fprintf(ff, "%.6lf", delta);
    fclose(ff);
}