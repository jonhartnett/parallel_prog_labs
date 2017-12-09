#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "io-util.h"

typedef struct IntArr{
    int* data;
    size_t length;
} IntArr;

IntArr NewArr(size_t len){
    IntArr arr = {calloc(sizeof(int), len), len};
    return arr;
}

void AddArr(IntArr x, IntArr y){
    if(x.length != y.length)
    {
        printf("Cannot add IntArrs of different lengths!");
        exit(1);
    }
    for(size_t i = 0; i < x.length; i++)
        x.data[i] += y.data[i];
}

int main(){

    //prompt for input
    size_t data_count;
    float* data;
    float min_meas, max_meas;
    int bin_count;
    {
        StreamIterator iter = allocStreamIter(stdin);

        printf("Data (list of floats):\n");
        data = readFloat32Array(iter, &data_count);

        printf("Minimum value:\n");
        min_meas = readFloat32(iter);
        printf("Maximum value:\n");
        max_meas = readFloat32(iter);
        printf("Number of bins:\n");
        bin_count = readInt32(iter);

        freeStreamIter(iter);
    }

    float* bin_maxes = array(float, data_count);
    IntArr bin_counts = NewArr(data_count);
    float bin_width = (max_meas - min_meas) / bin_count;

    for(int i = 0; i < bin_count; i++){
        bin_maxes[i] = min_meas + bin_width * (i + 1);
        bin_counts.data[i] = 0;
    }

    //declare a custom reduction so we don't have performance issues from cache invalidation
    #pragma omp declare reduction \
        (arr_add:IntArr:AddArr(omp_out, omp_in)) \
        initializer(omp_priv=NewArr(omp_orig.length))

    //calculate the bins in parallel
    #pragma omp parallel for reduction(arr_add:bin_counts)
    for(int i = 0; i < data_count; i++){
        float datum = data[i];
        int bin = (int)((datum - min_meas) / bin_width);
        if(bin > 0 && bin <= bin_counts.length)
            bin_counts.data[bin]++;
    }

    //print results
    printf("[%.2f, %.2f): %d\n", min_meas, bin_maxes[0], bin_counts.data[0]);
    for(int i = 1; i < bin_count; i++)
        printf("[%.2f, %.2f): %d\n", bin_maxes[i - 1], bin_maxes[i], bin_counts.data[i]);

    free(data);
    free(bin_maxes);
    free(bin_counts.data);
}