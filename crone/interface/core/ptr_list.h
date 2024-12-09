
typedef struct ptr_list {
    size_t count;
    size_t capacity;
    void* *data;
} ptr_list;

ptr_list ptrs_allocate(size_t initial_capacity);

void ptrs_append(ptr_list *list, void *item);

void ptrs_cleanup(ptr_list list);

