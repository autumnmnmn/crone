
typedef struct lookup {
    list *buckets;
    size_t datum_size;
    size_t capacity;
} lookup;

lookup lookup_allocate(size_t capacity, size_t datum_size);

bool lookup_get(lookup l, string s, hash h, void *datum);

void lookup_insert(lookup l, string s, hash h, void *datum);

void lookup_cleanup(lookup l);

