
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <window.h>
#include <vk.h>

int main() {
    window_handle window = window_getWindow();

    vulkan_handle vk = vulkan_init();

    while (!window_shouldClose(window)) {
        window_pollEvents(window);
    }

    vulkan_cleanup(vk);

    window_cleanup(window);

    return 0;
}

