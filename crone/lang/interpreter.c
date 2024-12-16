
#include <unistd.h>
#include <ctype.h>

#include <lang.h>
#include <core.h>

#define STATES(OP) \
    OP(OUTER_SPACE) \
    OP(TERM) \
    OP(PRELUDIC_SPACE) \
    OP(PRELUDE) \
    OP(BLOCK) \

#define COMMA_SEPARATED(VALUE) VALUE,
#define COMMA_SEPARATED_STRINGS(VALUE) #VALUE,

typedef enum parser_state {
    STATES(COMMA_SEPARATED)
} parser_state;

static const char *state_names[] = {
    STATES(COMMA_SEPARATED_STRINGS)
};

void print_substring(char *start, size_t length) {
    fprintf(stderr, "%.*s", (int)length, start);
}

bool ends_prelude(char c) {
    return c == ';' || c == '{';
}

bool starts_term(char c) {
    return !isspace(c);
}

bool ends_term(char c) {
    return isspace(c);
}

bool ends_block(char c) {

    return c == '}';
}

typedef struct parse {
    char *start;
    size_t length;
    parser_state parsed_as;
    void *subparse;
} parse;

void print_parse(parse p) {
    fprintf(stderr, "parsed as %s: \"", state_names[p.parsed_as]);
    print_substring(p.start, p.length);
    fprintf(stderr, "\"\n");
}

void execute(char *crone_script, size_t length) {

    parser_state state = OUTER_SPACE;

    ptr_list substrings = ptrs_allocate(512);

    char *start = crone_script;

    size_t sublength = 0;

    for (size_t index = 0; index < length; ++index) {
        // TODO: actual unicode parsing lol
        char _char = crone_script[index];
        switch (state) {
            // TODO: simplify entire state machine to be less repetitive
            case OUTER_SPACE:
                if (starts_term(_char)) {
                    parse *p = malloc(sizeof(parse));
                    p->start = start;
                    p->length = sublength;
                    p->parsed_as = state;
                    ptrs_append(&substrings, p);
                    start = (void*)(crone_script + index);
                    sublength = 0;
                    state = TERM;
                } else {
                    ++sublength;
                }
            break;
            case PRELUDIC_SPACE:
                ++sublength;
                if (starts_term(_char)) {
                    parse *p = malloc(sizeof(parse));
                    p->start = start;
                    p->length = sublength;
                    p->parsed_as = state;
                    ptrs_append(&substrings, p);
                    start = (void*)(crone_script + index);
                    sublength = 0;
                    state = PRELUDE;
                }
            break;
            case TERM:
                ++sublength;
                if (ends_term(_char)) {
                    parse *p = malloc(sizeof(parse));
                    p->start = start;
                    p->length = sublength;
                    p->parsed_as = state;
                    ptrs_append(&substrings, p);
                    start = (void*)(crone_script + index);
                    sublength = 0;
                    state = PRELUDIC_SPACE;
                }
            break;
            case PRELUDE:
                ++sublength;
                if (ends_prelude(_char)) {
                    parse *p = malloc(sizeof(parse));
                    p->start = start;
                    p->length = sublength;
                    p->parsed_as = state;
                    ptrs_append(&substrings, p);
                    start = (void*)(crone_script + index);
                    sublength = 0;
                    state = BLOCK;
                }
            break;
            case BLOCK:
                // TODO parsing for block dependent on term and prelude
                ++sublength;
                if (ends_block(_char)) {
                    parse *p = malloc(sizeof(parse));
                    p->start = start;
                    p->length = sublength;
                    p->parsed_as = state;
                    ptrs_append(&substrings, p);
                    start = (void*)(crone_script + index);
                    sublength = 0;
                    state = OUTER_SPACE;
                }
            break;
        }
    }

    parse *p = malloc(sizeof(parse));
    p->start = start;
    p->length = sublength;
    p->parsed_as = state;
    ptrs_append(&substrings, p);


    for (int i = 0; i < substrings.count; ++i) {
        print_parse(*(parse *)substrings.data[i]);
    }

    for (int i = 0; i < substrings.count; ++i) {
        free(substrings.data[i]);
    }

    ptrs_cleanup(substrings);

}

/*

# general syntax:

term prelude {
    block
}

or

term prelude;

# example:

## code
exposed void ptrs_cleanup(ptr_list list) {
    free(list.data);
}

## parse
term="exposed"
prelude="void ptrs_cleanup(ptr_list list)"
block="free(list.data);"


*/


