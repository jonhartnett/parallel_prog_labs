#include <stdio.h>
#include <omp.h>
#include <stdint.h>
#include <math.h>
#include "io-util.h"
#include "rand.h"

#define PI 3.14159265358979323846

int main(){
    StreamIterator iter = allocStreamIter(stdin);

    printf("Enter Tosses: \n");
    uint64_t tosses = readUInt64(iter);

    freeStreamIter(iter);

    if(tosses == 0){
        printf("Invalid number of tosses.");
        return 1;
    }

    const uint64_t seedVal = 0;
    Random rand;
#pragma omp parallel private(rand)
    rand = seed((uint64_t)omp_get_thread_num() ^ seedVal);

    uint64_t hits = 0;
#pragma omp parallel for schedule(static) reduction(+:hits) private(rand)
    for(uint64_t toss = 0; toss < tosses; toss++){
        double x = randFloat64(&rand, -1, 1);
        double y = randFloat64(&rand, -1, 1);
        if(x * x + y * y <= 1)
            hits++;
    }

    double pi = 4.0 * hits / tosses;
    double error = fabs(PI - pi) / PI;
    printf("PI: %lf\n", pi);
    printf("Error: %.6lf%%\n", error * 100);
}