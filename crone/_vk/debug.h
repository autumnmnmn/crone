
#include <string.h>
#include <stdio.h>

const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

static bool validationLayersSupported() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    size_t requestedLayerCount = sizeof(validationLayers) / sizeof(const char*);

    for (int reqIndex = 0; reqIndex < requestedLayerCount; ++reqIndex) {
        bool layerFound = false;
        const char* layerName = validationLayers[reqIndex];

        for (int availIndex = 0; availIndex < layerCount; ++availIndex) {
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

    fprintf(stderr, "[VK_DEBUG] %s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

