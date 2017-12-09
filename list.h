#ifndef PARALLEL_PROG_LABS_LIST_H
#define PARALLEL_PROG_LABS_LIST_H

#define ALLOC_LIST(TYPE, NAME, SIZE) \
    struct{ \
        size_t capacity; \
        size_t count; \
        TYPE* data; \
        size_t dataSize; \
    } NAME = {SIZE, 0, (TYPE*)malloc(sizeof(TYPE) * SIZE), sizeof(TYPE) * SIZE}

#define ADD_LIST(NAME, VALUE) \
    if(NAME.count++ > NAME.capacity){ \
        NAME.capacity *= 2; \
        NAME.dataSize *= 2; \
        NAME.data = (typeof(NAME.data))realloc(NAME.data, NAME.dataSize * NAME.capacity); \
    } \
    NAME.data[NAME.count - 1] = VALUE

#define SHRINK_LIST(NAME) realloc(NAME.data, NAME.dataSize * NAME.count)
#define FREE_LIST(NAME) free(NAME.data)

#endif //PARALLEL_PROG_LABS_LIST_H
