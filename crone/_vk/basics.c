
#include ".h"

#ifdef DO_VALIDATION
#include "debug.h"
#endif

ptr_list getRequiredExtensions() {
    ptr_list list = ptrs_allocate(4);

    #ifdef DO_VALIDATION
    ptrs_append(&list, "VK_EXT_debug_utils");
    #endif

    ptrs_append(&list, "VK_KHR_surface");

    #ifdef USE_X11
    ptrs_append(&list, "VK_KHR_xlib_surface");
    #endif
    return list;
}

void* vulkan_init() {
    vulkan_state *vk = malloc(sizeof(vulkan_state));
    if (vk == NULL) { CRASH("failed malloc: vulkan state"); }

    vk->pAllocator = NULL;

    #ifdef DO_VALIDATION
    if (!validationLayersSupported()) {
        CRASH("validation layers required but not supported");
    }
    #endif

    applicationInfo(appInfo,
        .pApplicationName = "vulkan app",
        .applicationVersion = VK_MAKE_VERSION(0,0,0),
        .pEngineName = "Crone",
        .engineVersion = VK_MAKE_VERSION(0,0,0),
        .apiVersion = VK_API_VERSION_1_0,
    );

    ptr_list requiredExtensions = getRequiredExtensions();

    instanceInfo(createInfo,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = requiredExtensions.count,
        .ppEnabledExtensionNames = (const char **) requiredExtensions.data
    );

    #ifdef DO_VALIDATION
    debugUtilsMessengerInfo(debugCreateInfo,
        .messageSeverity =
            //VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = &onDebugMessage,
    );

    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.pNext = &debugCreateInfo;
    #else
    createInfo.enabledLayerCount = 0;
    #endif

    if (vkCreateInstance(&createInfo, vk->pAllocator, &vk->instance) != VK_SUCCESS) {
        CRASH("failed to create vulkan instance");
    }

    ptrs_cleanup(requiredExtensions);

    #ifdef DO_VALIDATION
    PFN_vkCreateDebugUtilsMessengerEXT createFn = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vk->instance, "vkCreateDebugUtilsMessengerEXT");
    if (createFn == NULL) {
        CRASH("failed to find debug messenger procAddr");
    }
    if (createFn(vk->instance, &debugCreateInfo, vk->pAllocator, &vk->debugMessenger) != VK_SUCCESS) {
        CRASH("failed to create debug messenger");
    }
    #endif

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vk->instance, &deviceCount, NULL);
    vk->physicalDevices = ptrs_allocate(deviceCount);
    vkEnumeratePhysicalDevices(vk->instance, &deviceCount, (VkPhysicalDevice*) vk->physicalDevices.data);

    return vk;
}

void vulkan_cleanup(void *vk_vp) {
    vulkan_state *vk = vk_vp;


    #ifdef DO_VALIDATION
    PFN_vkDestroyDebugUtilsMessengerEXT destroyFn = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vk->instance, "vkDestroyDebugUtilsMessengerEXT");
    if (destroyFn == NULL) {
        CRASH("failed to find debug messenger destroyer");
    }
    destroyFn(vk->instance, vk->debugMessenger, vk->pAllocator);
    #endif

    vkDestroyInstance(vk->instance, vk->pAllocator);

    ptrs_cleanup(vk->physicalDevices);
    free(vk);
}

void* graphics_init(void *vk, void *window) {
    return NULL;
}

void graphics_cleanup(void *graphics) {

}

