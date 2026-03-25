#include <iostream>
#include <vector>
#include "raylib.h"
#include "charts.h"

using namespace std;
int main() {

    Color Dark_Green = Color{ 20, 160, 133, 255 };

    const int screenWidth = 800;
    const int screenHeight = 600;
    const float recWidth = 100.0f;
    const float recHeight = 50.0f;
    const float FPS = 24.0f; 

    const float dt = 1.0f / 120.0f;
    float accumulator = 0.0f;
    
    float v = 0.0f;
    float a = 0.0f;
    float x_0 = (screenWidth / 2.0f) - (recWidth / 2.0f);
    float amp_0 = (screenHeight / 2.0f) - (recHeight / 2.0f);
    float amp = amp_0 - 40.0f;
    float k = 500;
    float g = 9.81f;
    float m = 5.0f;
    float c = 0.5f;
    float t = 0.0f;

    float temp = amp - 30.0f;

    float t_0 = 0.0f;
    float wych = 0.0f;

    vector<float> czas;
    vector<float> wychylenie;
    vector<float> przysp;
    przysp.push_back(a);
    double ST = 0.0f;

    InitWindow(screenWidth, screenHeight, "Sprezynka");
    SetTargetFPS(FPS);

    while (WindowShouldClose() == false) {
        
        float frameT = GetFrameTime();
        accumulator += frameT;

        while (accumulator >= dt)
        {
            a = (-k * (amp - amp_0) - c * v) / m;
            v += a * dt;
            amp += v * dt;

            ST += dt;

            czas.push_back(ST);
            wychylenie.push_back(amp_0 - amp);
            przysp.push_back(a);

            accumulator -= dt;
        }

        
        BeginDrawing();
        ClearBackground(Dark_Green);

        DrawLine(0, (screenHeight / 2), screenWidth, (screenHeight / 2), RED);
        DrawLine((screenWidth / 2), 0, (screenWidth / 2), screenHeight, RED);

        DrawLine((screenWidth / 2), amp, (screenWidth / 2), temp, BLUE);

        DrawRectangle((int)x_0, (int)amp, (int)recWidth, (int)recHeight, WHITE);
        

        EndDrawing();
    }

    ::CloseWindow();

    cout << "Proba: " << czas.size() << endl;

    WykWych(czas, wychylenie);
    WykPrzys(czas, przysp);



    return 0;
}