#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "shell32.lib")
#include "raylib.h"
#include "rlgl.h"

int main() {
    const int anchoPantalla = 800;
    const int altoPantalla = 600;
    InitWindow(anchoPantalla, altoPantalla, "Cubo Multicolor - Raylib + OpenGL (MSVC)");

    Camera3D camara = { 0 };
    camara.position = Vector3{ 3.0f, 3.0f, 3.0f };
    camara.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camara.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camara.fovy = 45.0f;
    camara.projection = CAMERA_PERSPECTIVE;

    float anguloRotacion = 0.0f;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        anguloRotacion += 1.0f;

        BeginDrawing();
            ClearBackground(Color{ 20, 40, 80, 255 });

            BeginMode3D(camara);
                DrawGrid(10, 1.0f);

                rlPushMatrix();
                rlRotatef(anguloRotacion, 1.0f, 1.0f, 0.0f);

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

                rlPopMatrix();
            EndMode3D();

            DrawText("Raylib + OpenGL (MSVC Fijo)", 10, 10, 20, RAYWHITE);
            DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}