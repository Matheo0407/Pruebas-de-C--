#define SDL_MAIN_HANDLED
#include <windows.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <iostream>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("OpenGL Nativo (Sin Glad) - MSVC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    std::cout << "=========================================" << std::endl;
    std::cout << "  ¡EXITO! OpenGL Corriendo sin GLAD" << std::endl;
    std::cout << "  Version del Driver: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "=========================================" << std::endl;

    bool running = true; SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) { if (event.type == SDL_QUIT) running = false; }
        glClearColor(0.2f, 0.4f, 0.3f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }
    SDL_GL_DeleteContext(glContext); SDL_DestroyWindow(window); SDL_Quit();
    return 0;
}
