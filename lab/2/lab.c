#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* calculate(int N, double leftBorder);
void diffScheme(double **A, double *Y, int N, double h);
void solution(double **A, double *Y, int n);

double* calculate(int N, double leftBorder) {

    double *Y = (double *) malloc(N * sizeof(double));
    Y[0] = 0;
    Y[N - 1] = leftBorder;
    for (int i = 0; i < N; i++)
        Y[i] = Y[0] + i * (Y[N - 1] - Y[0]) / (N - 1);

    double h = leftBorder / (N - 1);
    double **A = (double **)malloc((N - 2) * sizeof(double *));
    for (int i = 0; i < N - 2; i++)
        A[i] = (double *)malloc((N - 1) * sizeof(double));

    for (int i = 0; i < 3; i++) {
        diffScheme(A, Y, N, h);

        double* block = (double *)malloc((N - 2) * sizeof(double));
        for (int i = 0; i < N - 2; i++)
            block[i] = -(Y[i + 2] - 2 * Y[i + 1] + Y[i] - h * h / 12 * (Y[i + 2] + 10 * Y[i + 1] + Y[i]));

        for (int j = 0; j < N - 2; j++)
            A[j][N - 2] = block[j];

        solution(A, block, N - 2);
        for (int j = 0; j < N - 2; j++)
            Y[j + 1] += block[j];
    }

    return Y;
}

void diffScheme(double **A, double *Y, int N, double h) {
    for (int i = 0; i < N - 2; i++)
        for (int j = 0; j < N - 2; j++)
            A[i][j] = 0;

    for (int i = 0; i < N - 2; i++)
        A[i][i] = -2 - 5 * h * h * exp(-Y[i + 1]) / 6;

    for (int i = 0; i < N - 3; i++)
        A[i][i + 1] = 1 - (h * h / 12) * exp(-Y[i + 2]);

    for (int i = 1; i < N - 2; i++)
        A[i][i - 1] = 1 - (h * h / 12) * exp(-Y[i]);
}

void solution(double **A, double *Y, int N) {
    for (int k = 0; k < N - 1; k++) {
        for (int i = k + 1; i < N; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < N + 1; j++)
                A[i][j] -= factor * A[k][j];
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        Y[i] = A[i][N] / A[i][i];
        for (int j = 0; j < i; j++)
            A[j][N] -= A[j][i] * Y[i];
    }
}

int main(int argc, char **argv) {
    int N = atoi(argv[1]);
    double leftborder = atof(argv[2]);

    double *Y = calculate(N, leftborder);

    FILE *rec = fopen("lab.txt", "a");
    for (int i = 0; i < N; i++)
        fprintf(rec, "%lf ", i * leftborder / (N - 1));
    fprintf(rec, "\n");
    for (int i = 0; i < N; i++) {
        fprintf(rec, "%lf ", Y[i]);
    }
    fprintf(rec, "\n");
    fclose(rec);

    return 0;
}