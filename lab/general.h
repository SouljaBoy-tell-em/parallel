#define ISIZE 1000
#define JSIZE 1000
#define EHTALON "ethalon.txt"
#define EHTALON_PAR "ethalon_par.txt"
#define MAIN_3B "3b.txt"
#define MAIN_3B_PAR "3b_par.txt"

void annulate(double a[ISIZE][JSIZE]) {
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = 0;
        }
    }
}

void init(double a[ISIZE][JSIZE]) {
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i][j] = 10 * i + j;
        }
    }
}

void record(char *fileName, double a[ISIZE][JSIZE]) {
    FILE *ff = fopen(fileName,"w");

    for(int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            fprintf(ff, "%f ", a[i][j]);
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