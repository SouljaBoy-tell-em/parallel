#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int N = atoi(argv[1]);
    double sum = 0.0;

    #pragma omp parallel
    {
        double partSum = 0.0;
        #pragma omp for
        for (int i = 1; i <= N; ++i) {
            partSum += 1.0 / i;
        }

        #pragma omp critical
        {
            sum += partSum;
        }
    }

    printf("sum = %lf\n", sum);
}

// clang -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp task2.c -o task2 && ./task2 1000
