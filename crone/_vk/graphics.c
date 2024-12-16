
#include ".h"

graphics_handle graphics_init(vulkan_handle vulkan, window_handle window) {
    graphics_state *graphics = malloc(sizeof(graphics_state));
    if (graphics == NULL) { CRASH("failed malloc: graphics_state"); }

    graphics->vulkan = vulkan;
    graphics->window = window;

    return graphics;
}

void graphics_cleanup(graphics_handle graphics) {
    free(graphics);
}

