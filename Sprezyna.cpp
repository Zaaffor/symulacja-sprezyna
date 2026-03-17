#include <iostream>
#include "raylib.h"

using namespace std;

int main() {

    Color Dark_Green = Color{ 20, 160, 133, 255 };

    const int screenWidth = 800;
    const int screenHeight = 600;
    const float recWidth = 100.0f;
    const float recHeight = 50.0f;

    
    float v = 0.0f;
    float a = 0.0f;
    float x_0 = (screenWidth / 2.0f) - (recWidth / 2.0f);
    float amp_0 = (screenHeight / 2.0f) - (recHeight / 2.0f);
    float amp = amp_0 - 40.0f;
    float k = 500;
    float g = 9.81f;
    float m = 5.0f;
    float c = 0.5f;

    float temp = amp - 30.0f;
    

    // F = -k(x - x_0) - c*v



    InitWindow(screenWidth, screenHeight, "Sprezynka");
    SetTargetFPS(24);

    while (WindowShouldClose() == false) {
        BeginDrawing();
        ClearBackground(Dark_Green);

        DrawLine(0, (screenHeight / 2), screenWidth, (screenHeight / 2), RED);
        DrawLine((screenWidth / 2), 0, (screenWidth / 2), screenHeight, RED);

        DrawLine((screenWidth / 2), amp, (screenWidth / 2), temp, BLUE);
        
        DrawRectangle(x_0, amp, recWidth, recHeight, WHITE);

        float dt = GetFrameTime();
        a = (- k * (amp - amp_0)  - c * v) / m;
        v += a * dt;
        amp += v * dt;

        cout << "a: " << a << " v: " << v << " wychylenie: " << amp_0 - amp << endl;

        EndDrawing();
    }

    CloseWindow();
    return 0;
}