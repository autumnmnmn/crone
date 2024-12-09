
/* Macros to make vulkan a bit more pleasant to work with */

#define vkInfo(type, stype, name, ...) Vk ## type ## Info name = {.sType = VK_STRUCTURE_TYPE_ ## stype ## _INFO, __VA_ARGS__};
#define vkInfoKHR(type, stype, name, ...) Vk ## type ## InfoKHR name = {.sType = VK_STRUCTURE_TYPE_ ## stype ## _INFO_KHR, __VA_ARGS__};
#define vkCreateInfo(type, stype, name, ...) Vk ## type ## CreateInfo name = {.sType = VK_STRUCTURE_TYPE_ ## stype ## _CREATE_INFO, __VA_ARGS__};
#define vkCreateInfoExt(type, stype, name, ...) Vk ## type ## CreateInfoEXT name = {.sType = VK_STRUCTURE_TYPE_ ## stype ## _CREATE_INFO_EXT, __VA_ARGS__};
#define vkCreateInfoKhr(type, stype, name, ...) Vk ## type ## CreateInfoKHR name = {.sType = VK_STRUCTURE_TYPE_ ## stype ## _CREATE_INFO_KHR, __VA_ARGS__};
#define vkAllocateInfo(type, stype, name, ...) Vk ## type ## AllocateInfo name = {.sType = VK_STRUCTURE_TYPE_ ## stype ## _ALLOCATE_INFO, __VA_ARGS__};
#define vkBeginInfo(type, stype, name, ...) Vk ## type ## BeginInfo name = {.sType = VK_STRUCTURE_TYPE_ ## stype ## _BEGIN_INFO, __VA_ARGS__};

// CREATE
#define deviceQueueInfo(n, ...) vkCreateInfo(DeviceQueue, DEVICE_QUEUE, n, __VA_ARGS__)
#define deviceInfo(n, ...) vkCreateInfo(Device, DEVICE, n, __VA_ARGS__)
#define instanceInfo(n, ...) vkCreateInfo(Instance, INSTANCE, n, __VA_ARGS__)
#define shaderModuleInfo(n, ...) vkCreateInfo(ShaderModule, SHADER_MODULE, n, __VA_ARGS__)
#define imageViewInfo(n, ...) vkCreateInfo(ImageView, IMAGE_VIEW, n, __VA_ARGS__)
#define renderPassInfo(n, ...) vkCreateInfo(RenderPass, RENDER_PASS, n, __VA_ARGS__)
#define graphicsPipelineInfo(n, ...) vkCreateInfo(GraphicsPipeline, GRAPHICS_PIPELINE, n, __VA_ARGS__)
#define framebufferInfo(n, ...) vkCreateInfo(Framebuffer, FRAMEBUFFER, n, __VA_ARGS__)
#define commandPoolInfo(n, ...) vkCreateInfo(CommandPool, COMMAND_POOL, n, __VA_ARGS__)
#define semaphoreInfo(n, ...) vkCreateInfo(Semaphore, SEMAPHORE, n, __VA_ARGS__)
#define fenceInfo(n, ...) vkCreateInfo(Fence, FENCE, n, __VA_ARGS__)

// (ext)
#define debugUtilsMessengerInfo(n, ...) vkCreateInfoExt(DebugUtilsMessenger, DEBUG_UTILS_MESSENGER, n, __VA_ARGS__)
// (khr)
#define swapchainInfo(n, ...) vkCreateInfoKhr(Swapchain, SWAPCHAIN, n, __VA_ARGS__)
#define xlibSurfaceInfo(n, ...) vkCreateInfoKhr(XlibSurface, XLIB_SURFACE, n, __VA_ARGS__)

// (pipeline)
#define pipelineLayoutInfo(n, ...) vkCreateInfo(PipelineLayout, PIPELINE_LAYOUT, n, __VA_ARGS__)
#define pipelineShaderStageInfo(n, ...) vkCreateInfo(PipelineShaderStage, PIPELINE_SHADER_STAGE, n, __VA_ARGS__)
#define pipelineDynamicStateInfo(n, ...) vkCreateInfo(PipelineDynamicState, PIPELINE_DYNAMIC_STATE, n, __VA_ARGS__)
#define pipelineVertexInputStateInfo(n, ...) vkCreateInfo(PipelineVertexInputState, PIPELINE_VERTEX_INPUT_STATE, n, __VA_ARGS__)
#define pipelineInputAssemblyStateInfo(n, ...) vkCreateInfo(PipelineInputAssemblyState, PIPELINE_INPUT_ASSEMBLY_STATE, n, __VA_ARGS__)
#define pipelineViewportStateInfo(n, ...) vkCreateInfo(PipelineViewportState, PIPELINE_VIEWPORT_STATE, n, __VA_ARGS__)
#define pipelineRasterizationStateInfo(n, ...) vkCreateInfo(PipelineRasterizationState, PIPELINE_RASTERIZATION_STATE, n, __VA_ARGS__)
#define pipelineMultisampleStateInfo(n, ...) vkCreateInfo(PipelineMultisampleState, PIPELINE_MULTISAMPLE_STATE, n, __VA_ARGS__)
#define pipelineColorBlendStateInfo(n, ...) vkCreateInfo(PipelineColorBlendState, PIPELINE_COLOR_BLEND_STATE, n, __VA_ARGS__)

// ALLOCATE
#define commandBufferInfo(n, ...) vkAllocateInfo(CommandBuffer, COMMAND_BUFFER, n, __VA_ARGS__)

// BEGIN
#define commandBufferBeginInfo(n, ...) vkBeginInfo(CommandBuffer, COMMAND_BUFFER, n, __VA_ARGS__)
#define renderPassBeginInfo(n, ...) vkBeginInfo(RenderPass, RENDER_PASS, n, __VA_ARGS__)

// ETC
#define applicationInfo(n, ...) vkInfo(Application, APPLICATION, n, __VA_ARGS__)
#define submitInfo(n, ...) vkInfo(Submit, SUBMIT, n, __VA_ARGS__)
#define presentInfo(n, ...) vkInfoKHR(Present, PRESENT, n, __VA_ARGS__)
#define semaphoreWaitInfo(n, ...) vkInfo(SemaphoreWait, SEMAPHORE_WAIT, n, __VA_ARGS__)

