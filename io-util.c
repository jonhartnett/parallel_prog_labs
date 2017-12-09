#include <stdlib.h>
#include <errno.h>
#include "io-util.h"
#include "list.h"
#include "util.h"

StreamIterator allocStreamIter(FILE* stream){
    StreamIterator iter = malloc(sizeof(StreamIteratorStruct));
    iter->stream = stream;
    iter->line = NULL;
    iter->capacity = 0;
    return iter;
}

void freeStreamIter(StreamIterator iter){
    free(iter->line);
    free(iter);
}

int isChar(int c, const char* chars, int len){
    for(int i = 0; i < len; i++){
        if(c == chars[i])
            return 1;
    }
    return 0;
}

void addChar(StreamIterator iter, size_t i, char c){
    if(i >= iter->capacity){
        if(iter->capacity == 0)
            iter->capacity = 64;
        else
            iter->capacity *= 2;
        iter->line = realloc(iter->line, iter->capacity);
    }
    iter->line[i] = c;
}

void readUntil(StreamIterator iter, const char* ds, int dc, int skipLeading){
    int c;
    if(skipLeading){
        do{
            c = fgetc(stdin);
        }while(isChar(c, ds, dc));
    }else{
        c = fgetc(stdin);
        if(isChar(c, ds, dc)){
            addChar(iter, 0, '\0');
            return;
        }
    }
    if(c == EOF){
        printf("Unexpected end of input.");
        exit(1);
    }
    size_t i = 0;
    do{
        addChar(iter, i++, c);
        c = fgetc(stdin);
    }while(!isChar(c, ds, dc) && c != EOF);
    addChar(iter, i, '\0');
}

char* readLine(StreamIterator iter){
    readUntil(iter, "\n", 1, 0);
    return iter->line;
}

char* readToken(StreamIterator iter){
    readUntil(iter, " \t\n", 3, 1);
    return iter->line;
}

void split(char* str, char*** tokens, size_t* count){
    ALLOC_LIST(char*, list, 8);

    char* token = strtok(str, " \t\n");
    while(token != NULL){
        ADD_LIST(list, token);
        token = strtok(NULL, " \t\n");
    }

    *tokens = list.data;
    *count = list.count;
}

#define IMPLEMENT_TYPE(TYPE, NAME, CONV_TYPE, CONVERSION, RANGE_CHECK) \
    TYPE parse##NAME(char* token){ \
        CONV_TYPE i = CONVERSION(token); \
        if(errno == ERANGE || RANGE_CHECK){ \
            printf("Input out of range."); \
            exit(1); \
        } \
        return (TYPE)i; \
    } \
    TYPE read##NAME(StreamIterator iter){ \
        char* token = readToken(iter); \
        return parse##NAME(token); \
    } \
    TYPE* read##NAME##Array(StreamIterator iter, size_t* length){ \
        char* line = readLine(iter); \
        char** tokens; \
        split(line, &tokens, length); \
        TYPE* arr = array(TYPE, *length); \
        for(size_t i = 0; i < *length; i++) \
            arr[i] = parse##NAME(tokens[i]); \
        free(tokens); \
        return arr; \
    }
#define STR_TO_SINT(STR) strtoll(STR, NULL, 10)
#define STR_TO_UINT(STR) strtoull(STR, NULL, 10)
#define STR_TO_FLOAT(STR) strtold(STR, NULL)
#define IMPLEMENT_SIGNED_INT_TYPE(TYPE, NAME, CAP_NAME) IMPLEMENT_TYPE(TYPE, NAME, long long, STR_TO_SINT, i < CAP_NAME##_MIN || i > CAP_NAME##_MAX)
#define IMPLEMENT_UNSIGNED_INT_TYPE(TYPE, NAME, CAP_NAME) IMPLEMENT_TYPE(TYPE, NAME, long long unsigned, STR_TO_UINT, i > CAP_NAME##_MAX)
#define IMPLEMENT_FLOAT_TYPE(TYPE, NAME) IMPLEMENT_TYPE(TYPE, NAME, long double, STR_TO_FLOAT, 0)
    IMPLEMENT_SIGNED_INT_TYPE(int8_t, Int8, INT8)
    IMPLEMENT_SIGNED_INT_TYPE(int16_t, Int16, INT16)
    IMPLEMENT_SIGNED_INT_TYPE(int32_t, Int32, INT32)
    IMPLEMENT_SIGNED_INT_TYPE(int64_t, Int64, INT64)
    IMPLEMENT_UNSIGNED_INT_TYPE(uint8_t, UInt8, UINT8)
    IMPLEMENT_UNSIGNED_INT_TYPE(uint16_t, UInt16, UINT16)
    IMPLEMENT_UNSIGNED_INT_TYPE(uint32_t, UInt32, UINT32)
    IMPLEMENT_UNSIGNED_INT_TYPE(uint64_t, UInt64, UINT64)
    IMPLEMENT_FLOAT_TYPE(float, Float32)
    IMPLEMENT_FLOAT_TYPE(double, Float64)
#undef IMPLEMENT_TYPE
#undef STR_TO_SINT
#undef STR_TO_UINT
#undef STR_TO_FLOAT
#undef IMPLEMENT_SIGNED_INT_TYPE
#undef IMPLEMENT_UNSIGNED_INT_TYPE
#undef IMPLEMENT_FLOAT_TYPE