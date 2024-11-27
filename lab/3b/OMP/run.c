#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// clang -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp run.c -o run && ./run
int main() {
    FILE *file = fopen("ethalon_dataset_3b.txt", "w");
    fclose(file);
    file = fopen("par_dataset_3b.txt", "w");
    fclose(file);

    printf("THREAD LIMIT: %d\n", omp_get_max_threads());
    // for(int i = 1; i <= omp_get_max_threads(); i++) {
    for(int i = 1; i <= 4; i++) {
        char sys[10] = "./3b ", num_buf[10];
        snprintf(num_buf, sizeof(num_buf), "%d", i);
        strcat(sys, num_buf);
        system(sys);
    }

    system("python3 measure/histogram.py");
}
