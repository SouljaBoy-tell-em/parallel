#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {

    int value = 0;
    #pragma omp parallel
    {
        for(int i = omp_get_num_threads(); i >= 0; i--) {
            #pragma omp barrier {
            if (i == omp_get_thread_num()) {
                #pragma omp critical
                {
                    printf("thread %d -> %d\n", i, value);
                    value++;
                }
            }
        }
    }
}