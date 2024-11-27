#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *file = fopen("ethalon_dataset_1c.txt", "w");
    fclose(file);
    file = fopen("par_dataset_1c.txt", "w");
    fclose(file);
    file = fopen("1c.txt", "w");
    fclose(file);
    file = fopen("1c_par.txt", "w");
    fclose(file);

    // for(int i = 1; i <= omp_get_max_threads(); i++) {
    system("./1c");
    for(int i = 2; i <= 4; i++) {
        char sys1[100] = "mpic++ 1c_par.c -o 1c_par && mpiexec -n ", num_buf[10], sys2[100] = " ./1c_par";
        snprintf(num_buf, sizeof(num_buf), "%d", i);
        strcat(sys1, num_buf);
        strcat(sys1, sys2);
        system(sys1);
    }

    system("python3 measure/measure.py");
}