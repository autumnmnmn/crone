
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

const uint32_t compress_rounds = 2;
const uint32_t finalize_rounds = 4;

#define ROTL(x, r) ((x << r) | (x >> (64 - r)))
#define ROTR(x, r) ((x >> r) | (x << (64 - r)))

hash calculate_hash(string s) {
    uint8_t b = s.length % 256;
    uint64_t v0 = init_v0;
    uint64_t v1 = init_v1;
    uint64_t v2 = init_v2;
    uint64_t v3 = init_v3;

    fprintf(stderr, " %lx %lx %lx %lx \n", v0, v1, v2, v3);

    size_t position = 0;
    while (s.length - position > 7) {
        uint64_t m =
            ((uint64_t)s.data[position]) |
            ((uint64_t)s.data[position+1] << 8) |
            ((uint64_t)s.data[position+2] << 16) |
            ((uint64_t)s.data[position+3] << 24) |
            ((uint64_t)s.data[position+4] << 32) |
            ((uint64_t)s.data[position+5] << 40) |
            ((uint64_t)s.data[position+6] << 48) |
            ((uint64_t)s.data[position+7] << 56);

        v3 ^= m;
        fprintf(stderr, " %lx %lx %lx %lx \n", v0, v1, v2, v3);

        for (int round = 0; round < compress_rounds; ++round) {
            // SipRound, matching formatting of specification
            v0 += v1;            v2 += v3;
            v1 = ROTL(v1, 13);   v3 = ROTL(v3, 16);
            v1 ^= v0;            v3 ^= v2;
            v0 = ROTL(v0, 32);
            v2 += v1;            v0 += v3;
            v1 = ROTL(v1, 17);   v3 = ROTL(v3, 21);
            v1 ^= v2;            v3 ^= v0;
            v2 = ROTL(v2, 32);
        }
        fprintf(stderr, " %lx %lx %lx %lx \n", v0, v1, v2, v3);

        v0 ^= m;

        fprintf(stderr, " %lx %lx %lx %lx \n", v0, v1, v2, v3);

        position += 8;
    }

    uint64_t m = ((uint64_t)b << 56);
    size_t byte = 0;
    while (s.length - position > byte) {
        m |= ((uint64_t)s.data[position+byte] << (8*byte));
        ++byte;
    }

    v3 ^= m;

    for (int round = 0; round < compress_rounds; ++round) {
        // SipRound, matching formatting of specification
        v0 += v1;            v2 += v3;
        v1 = ROTL(v1, 13);   v3 = ROTL(v3, 16);
        v1 ^= v0;            v3 ^= v2;
        v0 = ROTL(v0, 32);
        v2 += v1;            v0 += v3;
        v1 = ROTL(v1, 17);   v3 = ROTL(v3, 21);
        v1 ^= v2;            v3 ^= v0;
        v2 = ROTL(v2, 32);
    }

    v0 ^= m;

    v2 ^= (uint64_t)0xff;

    fprintf(stderr, " %lx %lx %lx %lx \n", v0, v1, v2, v3);

    for (int round = 0; round < finalize_rounds; ++round) {
        // SipRound, matching formatting of specification
        v0 += v1;            v2 += v3;
        v1 = ROTL(v1, 13);   v3 = ROTL(v3, 16);
        v1 ^= v0;            v3 ^= v2;
        v0 = ROTL(v0, 32);
        v2 += v1;            v0 += v3;
        v1 = ROTL(v1, 17);   v3 = ROTL(v3, 21);
        v1 ^= v2;            v3 ^= v0;
        v2 = ROTL(v2, 32);
    }

    fprintf(stderr, " %lx %lx %lx %lx \n", v0, v1, v2, v3);

    fprintf(stderr, " %lx \n", v0 ^ v1 ^ v2 ^ v3);

    return v0 ^ v1 ^ v2 ^ v3;
}

// TODO maybe-more-performant version ingesting one u8 at a time

