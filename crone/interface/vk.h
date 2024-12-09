
#include <window.h>

typedef void* vulkan_handle;

typedef void* graphics_handle;

typedef void* compute_handle;

vulkan_handle vulkan_init();

void vulkan_cleanup(vulkan_handle vulkan);

graphics_handle graphics_init(vulkan_handle vulkan, window_handle window);

void graphics_cleanup(graphics_handle graphics);

