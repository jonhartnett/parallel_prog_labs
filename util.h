#ifndef PARALLEL_PROG_LABS_UTIL_H
#define PARALLEL_PROG_LABS_UTIL_H

#define array(TYPE, SIZE) (TYPE*)malloc(sizeof(TYPE) * (SIZE))

#define DECLARE_TIMED_VARIABLES() struct timespec start, end;
#define START_TIMED() clock_gettime(CLOCK_MONOTONIC, &start);
#define END_TIMED() clock_gettime(CLOCK_MONOTONIC, &end);
#define TIME_ELAPSED() ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9)
#define MILLIS_ELAPSED() (uint64_t)round(TIME_ELAPSED() * 1000)

#endif //PARALLEL_PROG_LABS_UTIL_H
