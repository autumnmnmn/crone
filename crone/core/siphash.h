
/* SipHash magic numbers */
const uint64_t magic_number_0 = 0x736f6d6570736575;
const uint64_t magic_number_1 = 0x646f72616e646f6d;
const uint64_t magic_number_2 = 0x6c7967656e657261;
const uint64_t magic_number_3 = 0x7465646279746573;

/* insecurely stored keys bc nobody's trying to hashdos my interpreter */
const uint64_t key_0 = 0x0706050403020100;//0xdeadbeefdeadbeef;
const uint64_t key_1 = 0x0f0e0d0c0b0a0908;//acdc1234f0069420;

const uint64_t init_v0 = key_0 ^ magic_number_0;
const uint64_t init_v1 = key_1 ^ magic_number_1;
const uint64_t init_v2 = key_0 ^ magic_number_2;
const uint64_t init_v3 = key_1 ^ magic_number_3;

#define ROTL(x, r) ((x << r) | (x >> (64 - r)))

#define SIPROUND \
    v0 += v1; \
    v2 += v3; \
    v1 = ROTL(v1, 13); \
    v3 = ROTL(v3, 16); \
    v1 ^= v0; \
    v3 ^= v2; \
    v0 = ROTL(v0, 32); \
    v2 += v1; \
    v0 += v3; \
    v1 = ROTL(v1, 17); \
    v3 = ROTL(v3, 21); \
    v1 ^= v2; \
    v3 ^= v0; \
    v2 = ROTL(v2, 32);

#define COMPRESS_ROUNDS SIPROUND SIPROUND

#define FINALIZE_ROUNDS SIPROUND SIPROUND SIPROUND SIPROUND

typedef struct partial_hash {
    size_t length, position;
    uint64_t v0, v1, v2, v3;
    uint64_t chunk; // "m_n" in the SipHash paper
} partial_hash;

partial_hash start_hash() {
    #ifdef BIG_ENDIAN
    fprintf(stderr, "[core/hash.h] Warning: SipHash implementation expects a little endian system, but the BIG_ENDIAN flag was set for this target.");
    #endif
    partial_hash h = {
        .length = 0,
        .position = 0,
        .v0 = init_v0,
        .v1 = init_v1,
        .v2 = init_v2,
        .v3 = init_v3,
        .chunk = 0
    };
    return h;
}

partial_hash continue_hash(partial_hash partial, string s) {
    return start_hash();// todo
}

hash resolve_hash(partial_hash partial) {
    return partial.v0 ^ partial.v1 ^ partial.v2 ^ partial.v3;
}

hash compute_siphash_2_4(string s) {
    #ifdef BIG_ENDIAN
    fprintf(stderr, "[core/hash.h] Warning: SipHash implementation expects a little endian system, but the BIG_ENDIAN flag was set for this target.");
    #endif

    uint8_t b = s.length % 256;
    uint64_t v0 = init_v0;
    uint64_t v1 = init_v1;
    uint64_t v2 = init_v2;
    uint64_t v3 = init_v3;

    size_t position = 0;
    while (s.length - position > 7) {
        uint64_t m;
        memcpy(&m, &(s.data[position]), 8);

        v3 ^= m;

        COMPRESS_ROUNDS

        v0 ^= m;

        position += 8;
    }

    uint64_t m = ((uint64_t)b << 56);
    memcpy(&m, &(s.data[position]), s.length - position);

    v3 ^= m;

    COMPRESS_ROUNDS

    v0 ^= m;

    v2 ^= (uint64_t)0xff;

    FINALIZE_ROUNDS

    return v0 ^ v1 ^ v2 ^ v3;
}

