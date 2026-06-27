#define SDL_MAIN_HANDLED 
#include <SDL2/SDL.h>
#include <GL/gl.h>

int main(int argc, char* argv[]) {
    // 1. Inicializar SDL2 y configurar ventana OpenGL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("OpenGL + SDL2 NATIVO - g++", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // 2. Configurar la matriz de perspectiva 3D básica
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Configurar una perspectiva simple sin librerías externas
    float fov = 60.0f * 3.14159265f / 180.0f;
    float aspect = 800.0f / 600.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;
    float f = 1.0f / tanf(fov / 2.0f);
    float projectionMatrix[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar + zNear) / (zNear - zFar), -1,
        0, 0, (2.0f * zFar * zNear) / (zNear - zFar), 0
    };
    glLoadMatrixf(projectionMatrix);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST); // Activar 3D real

    // 3. Bucle de renderizado
    bool running = true;
    SDL_Event event;
    float angulo = 0.0f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Limpiar pantalla y buffer 3D
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -3.0f); // Alejar el cubo para poder verlo
        glRotatef(angulo, 1.0f, 1.0f, 0.0f); // Rotar en los ejes X e Y
        angulo += 1.0f; // Velocidad de giro

        // 4. Dibujar las 6 caras del Cubo 3D con colores
        glBegin(GL_QUADS);
            // Cara frontal (Rojo)
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f,  0.5f); glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f,  0.5f);
            // Cara trasera (Verde)
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f); glVertex3f( 0.5f, -0.5f, -0.5f);
            // Cara superior (Azul)
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f,  0.5f, -0.5f); glVertex3f(-0.5f,  0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f( 0.5f,  0.5f, -0.5f);
            // Cara inferior (Amarillo)
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f); glVertex3f(-0.5f, -0.5f,  0.5f);
            // Cara derecha (Cian)
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex3f( 0.5f, -0.5f, -0.5f); glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f( 0.5f, -0.5f,  0.5f);
            // Cara izquierda (Magenta)
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);
        glEnd();

        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
