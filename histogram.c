#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "io-util.h"

int main(){
    StreamIterator iter = allocStreamIter(stdin);

    size_t data_count;
    printf("Data (list of floats):\n");
    float* data = readFloat32Array(iter, &data_count);

    printf("Minimum value:\n");
    float min_meas = readFloat32(iter);
    printf("Maximum value:\n");
    float max_meas = readFloat32(iter);
    printf("Number of bins:\n");
    int bin_count = readInt32(iter);

    freeStreamIter(iter);

    float* bin_maxes = array(float, data_count);
    int* bin_counts = array(int, data_count);

    float bin_width = (max_meas - min_meas) / bin_count;

    for(int i = 0; i < bin_count; i++){
        bin_maxes[i] = min_meas + bin_width * (i + 1);
        bin_counts[i] = 0;
    }

#pragma omp parallel for
    for(int i = 0; i < data_count; i++){
        float datum = data[i];
        int bin = (int)((datum - min_meas) / bin_width);
#pragma omp atomic
        bin_counts[bin]++;
    }

    printf("[%.2f, %.2f): %d\n", min_meas, bin_maxes[0], bin_counts[0]);
    for(int i = 1; i < bin_count; i++)
        printf("[%.2f, %.2f): %d\n", bin_maxes[i - 1], bin_maxes[i], bin_counts[i]);

    free(data);
    free(bin_maxes);
    free(bin_counts);
}