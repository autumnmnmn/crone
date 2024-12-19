
typedef struct list {
    size_t count;
    size_t capacity;
    size_t element_size;
    void* *data;
} list;

list list_allocate(size_t initial_capacity, size_t element_size);

void list_append(list *list, void *item);

void *list_element(list list, size_t index);

void list_cleanup(list list);

