
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <crone.h>

const uint8_t test_str[15] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E
};

int main() {

    string s = {
        .data = (uint8_t*)test_str,
        .length = 15
    };

    hash h = calculate_hash(s);

    fprintf(stderr, " hash: %lx \n", h);

    /* parser
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

    char *file = mmap(0, fileStat.st_size, PROT_READ, MAP_SHARED, fileDesc, 0); // TODO read docs

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
    */


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

