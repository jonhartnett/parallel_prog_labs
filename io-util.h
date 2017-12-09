#ifndef PARALLEL_PROG_LABS_IO_UTIL_H
#define PARALLEL_PROG_LABS_IO_UTIL_H

#include <lzma.h>
#include <stdio.h>
#include <ctype.h>
#include <memory.h>

typedef struct StreamIteratorStruct{
    FILE* stream;
    char* line;
    size_t capacity;
} StreamIteratorStruct;

typedef StreamIteratorStruct* StreamIterator;

StreamIterator allocStreamIter(FILE* stream);
void freeStreamIter(StreamIterator iter);
char* readLine(StreamIterator iter);
char* readToken(StreamIterator iter);
void split(char* str, char*** tokens, size_t* count);

#define DECLARE_TYPE(TYPE, NAME) \
    TYPE read##NAME(StreamIterator iter); \
    TYPE* read##NAME##Array(StreamIterator iter, size_t* length);
    DECLARE_TYPE(int8_t, Int8)
    DECLARE_TYPE(int16_t, Int16)
    DECLARE_TYPE(int32_t, Int32)
    DECLARE_TYPE(int64_t, Int64)
    DECLARE_TYPE(uint8_t, UInt8)
    DECLARE_TYPE(uint16_t, UInt16)
    DECLARE_TYPE(uint32_t, UInt32)
    DECLARE_TYPE(uint64_t, UInt64)
    DECLARE_TYPE(float, Float32)
    DECLARE_TYPE(double, Float64)
#undef DECLARE_TYPE


#endif //PARALLEL_PROG_LABS_IO_UTIL_H
