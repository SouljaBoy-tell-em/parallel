#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include "general.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void record(FILE *ff, double **a) {
    for(int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            fprintf(ff, "%lf ", a[i][j]);
        }
        fprintf(ff, "\n");
    }

    fclose(ff);
}

void writeData(const char * file_name, double value) {
    FILE *ff = fopen(file_name, "a");
    if (ff != NULL) {
        fprintf(ff, "%.6f ", value);
    }
    fclose(ff);
}

int main(int argc, char **argv) {
    FILE *file = fopen("1c_par.txt", "w");
    fclose(file);

    double a[ISIZE][JSIZE];
    FILE *ff;

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

    t_start = MPI_Wtime();
    for (int i = 2; i < ISIZE; i++) {
        for (int j = 0; j < JSIZE - 3; j++) {
            a[i][j] = sin(5 * a[i - 2][j + 3]);
        }
    }
//    t_end = MPI_Wtime();
    MPI_Bcast(a, ISIZE * JSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    MPI_Barrier(MPI_COMM_WORLD);

    if(ISIZE % size > rank)
    {
        i_start = (ISIZE / size) * rank + rank;
        if (i_start == 0) {
            i_start = 2;
        }

        i_end = (ISIZE / size) * (rank + 1) + rank + 1;

        len = i_end - i_start;
        line_array = (double *) malloc(len * (JSIZE - 3) * sizeof(double));
    }
    else
    {
        i_start = (ISIZE / size) * rank + (ISIZE % size);
        if (i_start == 0) {
            i_start = 2;
        }

        i_end = (ISIZE / size) * (rank + 1) + (ISIZE % size);

        len = i_end - i_start;
        line_array = (double *) malloc(len * (JSIZE - 3) * sizeof(double));
    }

    for (int i = i_start; i < i_end; i++) {
        for (int j = 0; j < JSIZE - 3; j++) {
            line_array[(i - i_start) * (JSIZE - 3) + j] = sin(5 * a[i - 2][j + 3]);
        }
    }
    t_end = MPI_Wtime();

    if (rank != 0) {
        MPI_Send(line_array, (i_end - i_start) * (JSIZE - 3), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        FILE *ff = fopen("1c_par.txt", "a");

        double **arr = (double **) malloc(ISIZE * sizeof(double *));
        for(int i = 0; i < ISIZE; i++){
            arr[i] = (double *) malloc(JSIZE * sizeof(double));
            for(int j = 0; j < JSIZE; j++){
                if(i < 2 || j >= JSIZE - 3){
                    arr[i][j] = a[i][j];
                }
            }
        }

        for (int i = 0; i < len * (JSIZE - 3); i++) {
            arr[2 + i / (JSIZE - 3)][i % (JSIZE - 3)] = line_array[i];
        }

        int recv_s, recv_e;
        double *recv;
        for (int r = 1; r < size; r++) {
            if(ISIZE % size > r)
            {
                recv_s = (ISIZE / size) * r + r;
                if (recv_s == 0) {
                    recv_s = 2;
                }

                recv_e = (ISIZE / size) * (r + 1) + r + 1;
            }
            else
            {
                recv_s = (ISIZE / size) * r + (ISIZE % size);
                if (recv_s == 0) {
                    recv_s = 2;
                }

                recv_e = (ISIZE / size) * (r + 1) + (ISIZE % size);
            }

            int l = (recv_e - recv_s) * (JSIZE - 3);
            recv = (double *) malloc(l * sizeof(double));
            MPI_Recv(recv, l, MPI_DOUBLE, r, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < l; i++) {
                arr[(i / (JSIZE - 3)) + recv_s][i % (JSIZE - 3)] = recv[i];
            }
        }

        record(ff, arr);
        fclose(ff);
    }
//    t_end = MPI_Wtime();
    MPI_Finalize();

    if (rank == 0) {
        printf("par time: %lf sec\n", t_end - t_start);
        writeData("par_dataset_1c.txt", t_end - t_start);
    }
}