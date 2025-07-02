
#include <string.h>
#include <stdio.h>
#include <limits.h>

const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

static bool validationLayersSupported() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    // TODO: consider a malloc instead of VLA, or record basic justification for VLA
    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    size_t requestedLayerCount = sizeof(validationLayers) / sizeof(const char*);

    for (size_t reqIndex = 0; reqIndex < requestedLayerCount; ++reqIndex) {
        bool layerFound = false;
        const char* layerName = validationLayers[reqIndex];

        for (size_t availIndex = 0; availIndex < layerCount; ++availIndex) {
            VkLayerProperties layer = availableLayers[availIndex];

            if (strcmp(layerName, layer.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL onDebugMessage(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData) {

    (void)messageSeverity;
    (void)messageType;
    (void)pUserData;

    fprintf(stderr, "[_vk/debug.h] %s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

