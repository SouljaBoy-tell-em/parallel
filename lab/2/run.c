#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int threads_amount = atoi(argv[1]);
    FILE *f;
    if (threads_amount == 1) {
        f = fopen("lab.txt", "w");
        fclose(f);
    } else {
        f = fopen("lab_par.txt", "w");
        fclose(f);
    }

    for(double i = 0.1; i <= 1; i += 0.1) {
        char sys[50] = "./lab ", arg1_buf[10], arg2_buf[10];
        snprintf(arg1_buf, sizeof(arg1_buf), "%d ", 1000);
        snprintf(arg2_buf, sizeof(arg2_buf), "%lf ", i);
        strcat(sys, arg1_buf);
        strcat(sys, arg2_buf);
        strcat(sys, argv[1]);
        printf("%s\n", sys);
        system(sys);
    }

    system("python3 measure/measure.py");
}