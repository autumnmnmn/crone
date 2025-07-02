
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include <core.h>

#include "ptr_list.h"
#include "list.h"
#include "siphash.h"
#include "fnvhash.h"
#include "lookup.h"

hash compute_hash(string s) {
    return compute_siphash_2_4(s);
}

