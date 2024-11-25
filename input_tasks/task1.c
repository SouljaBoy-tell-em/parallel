#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    #pragma omp parallel
    {
        printf("Hello World... from thread = %d\n",
               omp_get_thread_num());
    }
}

// input_tasks % clang -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp task1.c -o task1 && ./task1
