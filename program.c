
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <crone.h>


int main() {

    string a = { .data = (uint8_t*)"apple", .length = 5 };
    string b = { .data = (uint8_t*)"banan", .length = 5 };
    string c = { .data = (uint8_t*)"cobra", .length = 5 };
    string d = { .data = (uint8_t*)"delta", .length = 5 };
    string e = { .data = (uint8_t*)"empty", .length = 5 };
    string f = { .data = (uint8_t*)"fruit", .length = 5 };

    lookup l = lookup_allocate(256, sizeof(string));

    lookup_insert(l, a, compute_siphash_2_4(a), &d);
    lookup_insert(l, b, compute_siphash_2_4(b), &e);
    lookup_insert(l, c, compute_siphash_2_4(c), &f);

    string o;

    if (lookup_get(l, a, compute_siphash_2_4(a), &o)) {
        fprintf(stderr, " a: %s \n", o.data);
    } else {
        CRASH("fail a\n");
    }
    if (lookup_get(l, b, compute_siphash_2_4(b), &o)) {
        fprintf(stderr, " b: %s \n", o.data);
    } else {
        CRASH("fail b\n");
    }
    if (lookup_get(l, c, compute_siphash_2_4(c), &o)) {
        fprintf(stderr, " c: %s \n", o.data);
    } else {
        CRASH("fail c\n");
    }

    lookup_cleanup(l);
    return 0;

    //int fileDesc = open("./crone/core/core.cr", O_RDONLY, 0);
    int fileDesc = open("./crone/lang/parseme.cr", O_RDONLY, 0);

    if (fileDesc == -1) {
        CRASH("bad file");
    }

    struct stat fileStat;
    int result = fstat(fileDesc, &fileStat);

    if (result == -1) {
        CRASH("bad fstat");
    }

    uint8_t *file = mmap(0, fileStat.st_size, PROT_READ, MAP_SHARED, fileDesc, 0); // TODO read docs

    if (file == NULL) {
        CRASH("bad mmap");
    }

    string script = {
        .data = file,
        .length = fileStat.st_size
    };

    execute_crone(script);

    result = munmap(file, fileStat.st_size);

    if (result == -1) {
        CRASH("bad munmap");
    }



    /* graphics
    window_handle window = window_getWindow();

    vulkan_handle vk = vulkan_init();
    graphics_handle graphics = graphics_init(vk, window);

    while (!window_shouldClose(window)) {
        window_pollEvents(window);
    }

    graphics_cleanup(graphics);
    vulkan_cleanup(vk);

    window_cleanup(window);

    fprintf(stderr, "[PROGRAM] done\n");

    return 0;
    // */
}

