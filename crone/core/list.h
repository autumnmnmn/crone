
list list_allocate(size_t initial_capacity, size_t element_size) {
    list list = {
        .count = 0,
        .capacity = initial_capacity,
        .element_size = element_size,
        .data = malloc(element_size * initial_capacity)
    };
    if (list.data == NULL) { CRASH("failed malloc: list"); }
    return list;
}

void list_append(list *list, void *item) {
    list->count += 1;
    if (list->count > list->capacity) {
        /*========* Next power of two *========*/
        list->capacity = list->count;
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
        list->data = realloc(list->data, list->element_size * list->capacity);
        if (list->data == NULL) { CRASH("failed realloc: list"); }
    }

    memcpy((uint8_t*)list->data + (list->count - 1) * list->element_size, item, list->element_size);
}

void *list_element(list list, size_t index) {
    return (uint8_t*)list.data + index * list.element_size;
}

void list_cleanup(list list) {
    free(list.data);
}

