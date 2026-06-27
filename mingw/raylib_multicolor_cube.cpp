#include "raylib.h"
#include "rlgl.h" // El motor OpenGL interno de Raylib

int main() {
    // 1. Inicializar la ventana
    const int anchoPantalla = 800;
    const int altoPantalla = 600;
    InitWindow(anchoPantalla, altoPantalla, "Cubo Multicolor - Raylib + OpenGL");

    // 2. Configurar la cámara 3D
    Camera3D camara = { 0 };
    camara.position = Vector3{ 3.0f, 3.0f, 3.0f }; // Posición en el espacio
    camara.target = Vector3{ 0.0f, 0.0f, 0.0f };   // Mirar al centro
    camara.up = Vector3{ 0.0f, 1.0f, 0.0f };       // Eje Y hacia arriba
    camara.fovy = 45.0f;                           // Campo de visión
    camara.projection = CAMERA_PERSPECTIVE;        // Perspectiva 3D real

    float anguloRotacion = 0.0f;
    SetTargetFPS(60); // Forzar 60 FPS estables

    // Bucle principal
    while (!WindowShouldClose()) {
        // Actualizar el ángulo para que gire continuamente
        anguloRotacion += 1.0f;

        // 3. Empezar a dibujar
        BeginDrawing();
            // Fondo Azul Eléctrico estilo Vulkan/DirectX limpio
            ClearBackground(Color{ 20, 40, 80, 255 });

            // Entrar al modo 3D de OpenGL
            BeginMode3D(camara);

                // Dibujar la cuadrícula del suelo
                DrawGrid(10, 1.0f);

                // Aplicar la rotación del cubo usando el motor interno rlgl
                rlPushMatrix();
                rlRotatef(anguloRotacion, 1.0f, 1.0f, 0.0f); // Rotar en X e Y

                // Dibujar las caras del cubo con vértices multicolores
                rlBegin(RL_QUADS);
                    // Cara frontal
                    rlColor3f(1.0f, 0.0f, 0.0f); rlVertex3f(-1.0f, -1.0f,  1.0f);
                    rlColor3f(0.0f, 1.0f, 0.0f); rlVertex3f( 1.0f, -1.0f,  1.0f);
                    rlColor3f(0.0f, 0.0f, 1.0f); rlVertex3f( 1.0f,  1.0f,  1.0f);
                    rlColor3f(1.0f, 1.0f, 0.0f); rlVertex3f(-1.0f,  1.0f,  1.0f);
                    // Cara trasera
                    rlColor3f(0.0f, 1.0f, 1.0f); rlVertex3f(-1.0f, -1.0f, -1.0f);
                    rlColor3f(1.0f, 0.0f, 1.0f); rlVertex3f(-1.0f,  1.0f, -1.0f);
                    rlColor3f(1.0f, 1.0f, 1.0f); rlVertex3f( 1.0f,  1.0f, -1.0f);
                    rlColor3f(0.0f, 0.0f, 0.0f); rlVertex3f( 1.0f, -1.0f, -1.0f);
                    // Cara superior
                    rlColor3f(1.0f, 0.0f, 0.0f); rlVertex3f(-1.0f,  1.0f, -1.0f);
                    rlColor3f(0.0f, 1.0f, 0.0f); rlVertex3f(-1.0f,  1.0f,  1.0f);
                    rlColor3f(0.0f, 0.0f, 1.0f); rlVertex3f( 1.0f,  1.0f,  1.0f);
                    rlColor3f(1.0f, 1.0f, 0.0f); rlVertex3f( 1.0f,  1.0f, -1.0f);
                    // Cara inferior
                    rlColor3f(0.0f, 1.0f, 1.0f); rlVertex3f(-1.0f, -1.0f, -1.0f);
                    rlColor3f(1.0f, 0.0f, 1.0f); rlVertex3f( 1.0f, -1.0f, -1.0f);
                    rlColor3f(1.0f, 1.0f, 1.0f); rlVertex3f( 1.0f, -1.0f,  1.0f);
                    rlColor3f(0.0f, 0.0f, 0.0f); rlVertex3f(-1.0f, -1.0f,  1.0f);
                    // Cara derecha
                    rlColor3f(1.0f, 0.0f, 1.0f); rlVertex3f( 1.0f, -1.0f, -1.0f);
                    rlColor3f(0.0f, 1.0f, 1.0f); rlVertex3f( 1.0f,  1.0f, -1.0f);
                    rlColor3f(1.0f, 1.0f, 0.0f); rlVertex3f( 1.0f,  1.0f,  1.0f);
                    rlColor3f(1.0f, 0.0f, 0.0f); rlVertex3f( 1.0f, -1.0f,  1.0f);
                    // Cara izquierda
                    rlColor3f(0.0f, 1.0f, 0.0f); rlVertex3f(-1.0f, -1.0f, -1.0f);
                    rlColor3f(0.0f, 0.0f, 1.0f); rlVertex3f(-1.0f, -1.0f,  1.0f);
                    rlColor3f(1.0f, 1.0f, 1.0f); rlVertex3f(-1.0f,  1.0f,  1.0f);
                    rlColor3f(0.5f, 0.5f, 0.5f); rlVertex3f(-1.0f,  1.0f, -1.0f);
                rlEnd();

                rlPopMatrix(); // Volver al estado de matriz original

            EndMode3D();

            // Texto informativo 2D en pantalla
            DrawText("Raylib + rlgl (OpenGL Core)", 10, 10, 20, RAYWHITE);
            DrawFPS(10, 40);

        EndDrawing();
    }

    // 4. Limpieza
    CloseWindow();
    return 0;
}
