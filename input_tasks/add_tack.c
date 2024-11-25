#include <CL/cl.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

const char* kernelSource = R"(
__kernel void countingSortByDigit(__global int* input, __global int* output, __global int* count, int size, int digit) {
    int base = 10; // Десятичная система
    int i;

    // Инициализация count
    for (i = 0; i < base; i++) {
        count[i] = 0;
    }

    // Подсчет количества элементов
    for (i = 0; i < size; i++) {
        int index = (input[i] / digit) % base;
        atomic_inc(&count[index]);
    }

    // Преобразование count[i] так, чтобы count[i] содержал позицию элемента в output[]
    for (i = 1; i < base; i++) {
        count[i] += count[i - 1];
    }

    // Построение выходного массива
    for (i = size - 1; i >= 0; i--) {
        int index = (input[i] / digit) % base;
        output[count[index] - 1] = input[i];
        count[index]--;
    }
}
)";

void radixSort(int* array, int size) {
    // Определение максимального значения
    int maxElement = *std::max_element(array, array + size);

    // Установка OpenCL
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    cl_mem d_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * sizeof(int), array, NULL);
    cl_mem d_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size * sizeof(int), NULL, NULL);
    cl_mem d_count = clCreateBuffer(context, CL_MEM_READ_WRITE, 10 * sizeof(int), NULL, NULL);

    for (int digit = 1; maxElement / digit > 0; digit *= 10) {
        // Запуск ядра
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_input);
        clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_output);
        clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_count);
        clSetKernelArg(kernel, 3, sizeof(int), &size);
        clSetKernelArg(kernel, 4, sizeof(int), &digit);

        size_t globalSize = size;
        clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

        // Копирование отсортированного массива обратно
        std::swap(d_input, d_output);
    }

    // Копируем результат обратно в хост
    clEnqueueReadBuffer(queue, d_input, CL_TRUE, 0, size * sizeof(int), array, 0, NULL, NULL);

    // Освобождение ресурсов
    clReleaseMemObject(d_input);
    clReleaseMemObject(d_output);
    clReleaseMemObject(d_count);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

int main() {
    const int size = 1024;
    int array[size];

    // Заполнение массива случайными числами
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 10000;
    }

    std::cout << "Массив до сортировки:n";
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    // Сортировка
    radixSort(array, size);

    std::cout << "Массив после сортировки:n";
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
