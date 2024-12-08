
#include "crone_util.h"

typedef struct ptr_list {
    size_t count;
    size_t capacity;
    void* *data;
} ptr_list;

static inline ptr_list ptrs_allocate(size_t initial_capacity) {
    ptr_list list = {
        .count = 0,
        .capacity = initial_capacity,
        .data = malloc(sizeof(void*) * initial_capacity)
    };
    if (list.data == NULL) { CRASH("failed malloc: ptr_list"); }
    return list;
}

void ptrs_append(ptr_list *list, void *item) {
    list->count += 1;
    if (list->count > list->capacity) {
        /*========* Next power of two *========*/
        --list->capacity;
        list->capacity |= list->capacity >> 1;
        list->capacity |= list->capacity >> 2;
        list->capacity |= list->capacity >> 4;
        list->capacity |= list->capacity >> 8;
        list->capacity |= list->capacity >> 16;
        if (sizeof(void*) > sizeof(uint32_t)) {
            list->capacity |= list->capacity >> 32;
        }
        ++list->capacity;
        /*=====================================*/

        list->data = realloc(list->data, sizeof(void*) * list->capacity);
        if (list->data == NULL) { CRASH("failed realloc: ptr_list"); }
    }

    list->data[list->count - 1] = item;
}

static inline void ptrs_cleanup(ptr_list list) {
    free(list.data);
}

