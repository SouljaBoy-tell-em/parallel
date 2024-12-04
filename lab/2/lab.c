#include <stdio.h>
// #include <omp.h>
#include <math.h>
#include <stdlib.h>

#define eps 0.00001
#define X0 -10.0
#define XLAST 10.0
#define Y0 sqrt(2)
#define YLAST sqrt(2)

double f(double a, double y) {
    return a * y * (y * y - 1);
}

void initA(double **A, double a, int N) {
    A[0][0] = f(a, Y0) / Y0;
    A[N - 1][N - 1] = f(a, YLAST) / YLAST;

    double h = (XLAST - X0) / (N - 1);
    // #pragma omp parallel for
        for (int i = 1; i < N - 1; i++) {
            A[i][i - 1] = 1 / (h * h);
            A[i][i] = -2 / (h * h);
            A[i][i + 1] = 1 / (h * h);
        }
}

void initF(double *F, double a, int N, double *Y) {
    F[0] = f(a, Y0);
    F[N - 1] = f(a, YLAST);
    for (int i = 1; i < N - 1; i++) {
        F[i] = f(a, Y[i + 1]) / 12 +  5 * f(a, Y[i]) / 6 + f(a, Y[i - 1]) / 12;
    }
}

double* solve(double** A, double* F, int N) {
    double* Y = (double*) malloc(N * sizeof(double)), max = 0;

    int index = 0;
    for(int k = 0; k < N; k++) {
        max = fabs(A[k][k]);
        index = k;
        for (int i = k + 1; i < N; i++) {
            if (fabs(A[i][k]) > max) {
                max = fabs(A[i][k]);
                index = i;
            }
        }

        if (max < eps)
            return 0;

        double temp = 0;
        for (int j = 0; j < N; j++) {
            temp = A[k][j];
            A[k][j] = A[index][j];
            A[index][j] = temp;
        }

        temp = F[k];
        F[k] = F[index];
        F[index] = temp;
        for (int i = k; i < N; i++) {
            temp = A[i][k];
            if (fabs(temp) < eps)
                continue;

            for (int j = k; j < N; j++)
                A[i][j] = A[i][j] / temp;
            F[i] = F[i] / temp;

            if (i == k)
                continue;

            for (int j = 0; j < N; j++)
                A[i][j] = A[i][j] - A[k][j];
            F[i] = F[i] - F[k];
        }
    }

    for (int k = N - 1; k >= 0; k--) {
        Y[k] = F[k];
        for (int i = 0; i < k; i++)
            F[i] = F[i] - A[i][k] * Y[k];
    }
    return Y;
}

// 1st arg - amount of points;
// 2nd arg - the parameter 'a';
int main(int argc, char **argv) {
    int N = atoi(argv[1]);
    double a = atof(argv[2]);

    double *F = (double *) malloc(N * sizeof(double));

    double *Y = (double *) malloc(N * sizeof(double));
    for (int i = 0; i < N; i++) {
        Y[i] = sqrt(2);
    }

    double **A = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *) malloc(N * sizeof(double));
    }

    initA(A, a, N);
    initF(F, a, N, Y);
    Y = solve(A, F, N);

    initF(F, a, N, Y);
    Y = solve(A, F, N);

    for (int i = 0; i < N; i++) {
        printf("Y[%d] = %lf\n", i, Y[i]);
    }
}