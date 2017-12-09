#include <float.h>
#include "rand.h"

Random seed(uint64_t seed){
    Random rand;
    rand.next = seed;
    return rand;
}
uint64_t custom_lcg(uint_fast64_t* next){
    return *next = *next * 2862933555777941757ULL + 3037000493ULL;
}
double randFloat64(Random* rand, double min, double max){
    union {
        double f;
        uint64_t i;
    } u;
    u.f = 1.0;
    u.i = u.i | (custom_lcg(&rand->next) >> (64 - (DBL_MANT_DIG-1)));
    double result = u.f - 1.0;
    return min + (result * (max - min + DBL_EPSILON));
}