
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <crone.h>

int main() {
    int fileDesc = open("./crone/core/core.cr", O_RDONLY, 0);

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

    execute(file, fileStat.st_size);

    result = munmap(file, fileStat.st_size);

    if (result == -1) {
        CRASH("bad munmap");
    }



    /*
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

