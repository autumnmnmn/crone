
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include <lang.h>
#include <core.h>

#define STATES(OP) \
    OP(INVALID) \
    OP(OUTER_SPACE) \
    OP(TERM) \
    OP(INNER_SPACE) \
    OP(PUNCTUATION) \
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
    for (size_t i = 0; i < length; ++i) {
        if (start[i] == '\n') {
            fputc('\\', stderr);
            fputc('n', stderr);
        } else {
            fputc(start[i], stderr);
        }
    }
}

typedef struct parse {
    char *start;
    size_t length;
    parser_state parsed_as;
    list subparses;
} parse;

const char *spaces = "                                            ";

void print_parse(parse p, size_t indent) {
    size_t space_count = 4 * indent;
    if (space_count > strlen(spaces)) {
        space_count = strlen(spaces);
    }
    fwrite(spaces, 1, space_count, stderr);
    fprintf(stderr, "%s[", state_names[p.parsed_as]);
    if (p.parsed_as != BLOCK) {
        print_substring(p.start, p.length);
    } else {
        fprintf(stderr, "%lu", p.subparses.count);
    }
    fprintf(stderr, "]\n");
}

void transition(list *l, parse *p, char *end_position, parser_state next_state) {
    p->length = (size_t)end_position - (size_t)(p->start);
    list_append(l,p);
    p->start = end_position;
    p->parsed_as = next_state;
}

// TODO UNICODE

list/*subparses*/ parse_crone(string crone_script, size_t *p_position) {
    list parses = list_allocate(128, sizeof(parse));

    parse current_parse;

    current_parse.start = crone_script.data + (*p_position);
    current_parse.parsed_as = OUTER_SPACE;

    #define NEXT(state) transition(&parses, &current_parse, crone_script.data + (*p_position), state)

    // optimize w/ computed goto?

    for (; (*p_position) < crone_script.length; ++(*p_position)) {
        char _char = crone_script.data[*p_position];

        switch (current_parse.parsed_as) {
            case OUTER_SPACE:
                if (_char == '}') {
                    NEXT(INVALID);
                } else if (_char == '{') {
                    NEXT(BLOCK);
                } else if (_char == ';') {
                    NEXT(PUNCTUATION);
                } else if (!isspace(_char)) {
                    NEXT(TERM);
                }
            break;
            case INNER_SPACE:
                if (_char == '{') {
                    NEXT(BLOCK);
                } else if (_char == ';') {
                    NEXT(PUNCTUATION);
                } else if (_char == '}') {
                    NEXT(INVALID);
                } else if (!isspace(_char)) {
                    NEXT(TERM);
                }
            break;
            case TERM:
                if (isspace(_char)) {
                    NEXT(INNER_SPACE);
                } else if (_char == '}') {
                    NEXT(INVALID);
                } else if (_char == '{') {
                    NEXT(BLOCK);
                } else if (_char == ';') {
                    NEXT(PUNCTUATION);
                }
            break;
            case BLOCK:
                // TODO parsing for block dependent on term and prelude
                current_parse.subparses = parse_crone(crone_script, p_position);
                NEXT(OUTER_SPACE);
            break;
            case PUNCTUATION:
                if (isspace(_char)) {
                    NEXT(OUTER_SPACE);
                } else if (_char == '}') {
                    NEXT(INVALID);
                } else if (_char == '{') {
                    NEXT(BLOCK);
                } else if (_char != ';') {
                    NEXT(TERM);
                }
            break;
            case INVALID:
                return parses;
            break;
        }
    }

    #undef NEXT

    list_append(&parses, &current_parse);

    return parses;
}

void print_parses(list parses, int indent) {
    for (int i = 0; i < parses.count; ++i) {
        parse p = *(parse*)list_element(parses, i);
        //if (p.parsed_as != OUTER_SPACE && p.parsed_as != INNER_SPACE) {
            print_parse(p, indent);
            if (p.parsed_as == BLOCK) {
                print_parses(p.subparses, indent + 1);
            }
        //}
    }
}

void cleanup_parses(list parses) {
    for (int i = 0; i < parses.count; ++i) {
        parse p = *(parse*)list_element(parses, i);
        if (p.parsed_as == BLOCK) {
            cleanup_parses(p.subparses);
        }
    }
    list_cleanup(parses);
}

void execute_crone(string crone_script) {
    size_t position = 0;
    list parses = parse_crone(crone_script, &position);

    print_parses(parses, 0);

    if (position < crone_script.length) {
        fprintf(stderr, "warning: early exit @ %lu / %lu\n", position, crone_script.length);
    }

    cleanup_parses(parses);
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


