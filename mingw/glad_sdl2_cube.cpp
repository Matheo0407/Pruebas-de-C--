#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <cmath>
#include <iostream>

// Shaders GLSL modernos embebidos
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;
    uniform mat4 transform;
    void main() {
        gl_Position = transform * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";

// Función simple para crear una matriz de proyección y rotación sin librerías externas
void getTransformMatrix(float angle, float aspect, float* matrix) {
    float c = cos(angle);
    float s = sin(angle);
    
    // Matriz combinada de rotación (X e Y) + perspectiva básica integrada
    for(int i=0; i<16; i++) matrix[i] = 0.0f;
    matrix[0] = c / aspect;  matrix[1] = s * s;       matrix[2] = -s * c;
    matrix[5] = c;           matrix[6] = s;
    matrix[8] = s / aspect;  matrix[9] = -s * c;      matrix[10] = c * c;
    matrix[14] = -2.5f;                               matrix[15] = 1.0f; // Alejar en Z
}

int main(int argc, char* argv[]) {
    // 1. Inicializar SDL2 y Ventana OpenGL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("GLAD + SDL2 - Cubo 3D Real en g++", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // 2. Inicializar GLAD (Usa el cargador de funciones de SDL)
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Fallo al inicializar GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // Activar buffer de profundidad para 3D

    // 3. Compilar Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 4. Geometría del Cubo (Posiciones X,Y,Z y Colores R,G,B por cada vértice)
    float vertices[] = {
        -0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,    0.5f,-0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,  0.0f, 0.0f, 1.0f,   -0.5f, 0.5f,-0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 1.0f,    0.5f,-0.5f, 0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f,   -0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f
    };
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,  4, 5, 6, 6, 7, 4,  4, 0, 3, 3, 7, 4,
        1, 5, 6, 6, 2, 1,  3, 2, 6, 6, 7, 3,  4, 5, 1, 1, 0, 4
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 5. Bucle de Renderizado
    bool running = true;
    SDL_Event event;
    float angle = 0.0f;
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Color de fondo azul oscuro
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Actualizar la rotación del cubo
        angle += 0.01f;
        float transform[16];
        getTransformMatrix(angle, 800.0f / 600.0f, transform);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    // Limpieza de memoria
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
