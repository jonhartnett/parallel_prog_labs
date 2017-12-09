#ifndef PARALLEL_PROG_RAND_H
#define PARALLEL_PROG_RAND_H

#include <stdint.h>

typedef struct Random{
    uint_fast64_t next;
} Random;
Random seed(uint64_t seed);
double randFloat64(Random* rand, double min, double max);

#endif //PARALLEL_PROG_RAND_H