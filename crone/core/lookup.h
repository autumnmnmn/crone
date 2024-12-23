
const size_t initial_bucket_capacity = 4;

lookup lookup_allocate(size_t capacity, size_t datum_size) {
    lookup l = {
        .buckets = malloc(capacity * sizeof(list)),
        .datum_size = datum_size,
        .capacity = capacity
    };

    if (l.buckets == NULL) {
        CRASH("failed malloc: lookup\n");
    }

    for (size_t i = 0; i < capacity; ++i) {
        l.buckets[i] = list_allocate(initial_bucket_capacity, sizeof(string) + datum_size);
    }

    return l;
}

// TODO move this elsewhere & expose it as part of core
bool match(string a, string b) {
    if (a.length != b.length) return false;

    return memcmp(a.data, b.data, a.length) == 0;
}

bool lookup_get(lookup l, string s, hash h, void *datum) {
    list bucket = l.buckets[h % l.capacity];
    for (size_t i = 0; i < bucket.count; ++i) {
        void *entry = list_element(bucket, i);
        string entry_string = *(string*)entry;
        if (match(entry_string, s)) {
            memcpy(datum, ((uint8_t*)entry) + sizeof(string), l.datum_size);
            return true;
        }
    }
    return false;
}

void lookup_insert(lookup l, string s, hash h, void *datum) {
    list *bucket = &(l.buckets[h % l.capacity]);
    for (size_t i = 0; i < bucket->count; ++i) {
        void *entry = list_element(*bucket, i);
        string entry_string = *(string*)entry;
        if (match(entry_string, s)) {
            memcpy(((uint8_t*)entry)+sizeof(string), datum, l.datum_size);
            return;
        }
    }

    uint8_t buffer[sizeof(string) + l.datum_size];

    *(string*)buffer = s;
    memcpy(buffer + sizeof(string), datum, l.datum_size);

    list_append(bucket, buffer);
}

void lookup_cleanup(lookup l) {
    for (size_t i = 0; i < l.capacity; ++i) {
        list_cleanup(l.buckets[i]);
    }
    free(l.buckets);
}

