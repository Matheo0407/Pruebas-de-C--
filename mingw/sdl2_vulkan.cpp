#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // 1. Inicializar SDL2 con soporte para Vulkan
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error al inicializar SDL2: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Crear la ventana configurada específicamente para Vulkan
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan + SDL2 - Inicialización Directa con g++",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_VULKAN
    );

    if (!window) {
        std::cout << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // 2. Configurar la Instancia de Vulkan
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Prueba Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Sin Motor";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Obtener las extensiones que SDL2 necesita para conectar Vulkan con Windows
    unsigned int extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    std::vector<const char*> extensions(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cout << "¡Error crítico! No se pudo crear la instancia de Vulkan." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 3. Detectar tu Tarjeta Gráfica (Physical Device)
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        std::cout << "Error: ¡No se encontraron tarjetas gráficas con soporte para Vulkan!" << std::endl;
        vkDestroyInstance(instance, nullptr);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Obtener el nombre de tu GPU principal
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(devices[0], &deviceProperties);

    std::cout << "=========================================" << std::endl;
    std::cout << "  ¡ÉXITO! Vulkan inicializado correctamente." << std::endl;
    std::cout << "  GPU Detectada: " << deviceProperties.deviceName << std::endl;
    std::cout << "=========================================" << std::endl;

    // 4. Bucle básico de la ventana para que no se cierre sola
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    // Limpieza de recursos de Vulkan y SDL2
    vkDestroyInstance(instance, nullptr);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
