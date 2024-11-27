#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <mpi.h>
#include "../general.h"

double a[ISIZE][JSIZE];

void record(FILE *ff, double *arr, int len) {
    for(int i = 0; i < len / ISIZE; i++) {
        for(int j = 0; j < JSIZE; j++) {
            fprintf(ff, "%lf ", arr[i * JSIZE + j]);
        }
        fprintf(ff, "\n");
    }
}

int main(int argc, char *argv[]) {
    remove("ethalon_par.txt");
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = 10 * i + j;
        }
    }


    int size;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int i_start, i_end, len;
    double t_start, t_end;
    double *line_array;

    if(ISIZE % size > rank)
    {
        i_start = (ISIZE / size) * rank + rank;
        i_end = (ISIZE / size) * (rank + 1) + rank + 1;
        len = i_end - i_start;
        line_array = (double *) malloc(len * JSIZE * sizeof(double));
    }
    else
    {
        i_start = (ISIZE / size) * rank + (ISIZE % size);
        i_end = (ISIZE / size) * (rank + 1) + (ISIZE % size);
        len = i_end - i_start;
        line_array = (double *) malloc(len * JSIZE * sizeof(double));
    }

    t_start = MPI_Wtime();
    for (int i = i_start; i < i_end; i++) {
        for (int j = 0; j < JSIZE; j++){
//            a[i][j] = sin(2 * a[i][j]);
            line_array[(i - i_start) * JSIZE + j] = sin(2 * a[i][j]);
        }
    }
    t_end = MPI_Wtime();
    if (rank != 0) {
        MPI_Send(line_array, (i_end - i_start) * JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        FILE *ff = fopen("ethalon_par.txt", "a");
        record(ff, line_array, len * JSIZE);

        int recv_s, recv_e;
        double *recv;

        for(int i = 1; i < size; i++){
            if(ISIZE % size > i)
            {
                recv_s = (ISIZE / size) * i + i;
                recv_e = (ISIZE / size) * (i + 1) + i + 1;
            }
            else
            {
                recv_s = (ISIZE / size) * i + (ISIZE % size);
                recv_e = (ISIZE / size) * (i + 1) + (ISIZE % size);
            }

            int l = (recv_e - recv_s) * JSIZE;
            recv = (double *) malloc(l * sizeof(double));
            MPI_Recv(recv, l, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            record(ff, recv, l);
        }

        fclose(ff);
    }
    MPI_Finalize();
    printf("Elapsed time: %f\n", t_end - t_start);
}