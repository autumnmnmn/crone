
typedef struct vulkan_state {
    VkInstance instance;
    ptr_list physicalDevices;
    VkAllocationCallbacks *pAllocator;
    #ifdef DO_VALIDATION
    VkDebugUtilsMessengerEXT debugMessenger;
    #endif
} vulkan_state;

typedef struct graphics_state {
    vulkan_handle vulkan;
    window_handle window;
} graphics_state;
