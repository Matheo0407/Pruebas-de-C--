#define SDL_MAIN_HANDLED
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

int main(int argc, char* argv[]) {
    HMODULE vulkanModule = LoadLibraryA("vulkan-1.dll");
    if (!vulkanModule) { std::cout << "No se encontro la GPU con Vulkan" << std::endl; return -1; }

    auto p_vkCreateInstance = (PFN_vkCreateInstance)GetProcAddress(vulkanModule, "vkCreateInstance");
    auto p_vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)GetProcAddress(vulkanModule, "vkEnumeratePhysicalDevices");
    auto p_vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)GetProcAddress(vulkanModule, "vkGetPhysicalDeviceProperties");

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Vulkan - MSVC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN);

    VkInstance instance; VkApplicationInfo appInfo{}; appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; appInfo.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo createInfo{}; createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; createInfo.pApplicationInfo = &appInfo;

    unsigned int extCount = 0; SDL_Vulkan_GetInstanceExtensions(window, &extCount, nullptr);
    std::vector<const char*> exts(extCount); SDL_Vulkan_GetInstanceExtensions(window, &extCount, exts.data());
    createInfo.enabledExtensionCount = extCount; createInfo.ppEnabledExtensionNames = exts.data();

    p_vkCreateInstance(&createInfo, nullptr, &instance);
    uint32_t devCount = 0; p_vkEnumeratePhysicalDevices(instance, &devCount, nullptr);
    std::vector<VkPhysicalDevice> devs(devCount); p_vkEnumeratePhysicalDevices(instance, &devCount, devs.data());

    VkPhysicalDeviceProperties props; p_vkGetPhysicalDeviceProperties(devs[0], &props);
    std::cout << "=========================================" << std::endl;
    std::cout << "  VULKAN MSVC: " << props.deviceName << std::endl;
    std::cout << "=========================================" << std::endl;

    bool running = true; SDL_Event event;
    while (running) { while (SDL_PollEvent(&event)) { if (event.type == SDL_QUIT) running = false; } }
    SDL_DestroyWindow(window); SDL_Quit();
    return 0;
}
