#include <iostream>
#include <vector>
#include "raylib.h"
#include "charts.h"


#define RAYGUI_IMPLEMENTATION
extern "C" {
    #include "raygui.h"
}

void Sinusoida(const std::vector<float>& dane, int x, int y, int w, int h)
{
    //DrawRectangleLines(x, y, w, h, BLACK);

    int przerwa = 4;
    int N = w / przerwa;
    int count = dane.size();
     
    int c_0 = std::max(0, count - N);
    int widoczne = count - c_0;
    float midy = y + h / 2.0f;

    int offset = (N - widoczne) * przerwa;

    int idx = 0;

    for (int i = c_0; i < count; i++)
    {
        float px = x + offset + idx * przerwa;
        float py = midy - dane[i];

        if (py >= y && py <= y + h)
        {
            DrawCircle(px, py, 2, DARKBLUE);
        }
        idx++;
    }
}

enum SimState {
    MENU,
    SIMULATION
};

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
    const float c_min = 0.0f;
    const float c_max = 5.0f;
    float t = 0.0f;

    float temp = amp - 30.0f;

    const int plotx = 0;
    const int plotw = screenWidth/2;
    const int ploth = 200;
    const int plotp = 400;
    const int ploty = (amp_0 + recHeight / 2.0f - ploth / 2.0f);

    float t_0 = 0.0f;
    float wych = 0.0f;

    vector<float> czas;
    vector<float> wychylenie;
    vector<float> przysp;
    przysp.push_back(a);
    double ST = 0.0f;

    SimState currSt = MENU;

    InitWindow(screenWidth, screenHeight, "Sprezynka");
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {

        if (currSt == MENU) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            
            DrawText("Symulacja sprezyny", 200, 150, 40, DARKGRAY);

            
            Rectangle startButton = { screenWidth / 2.0f - 100, 300, 200, 50 };
            if (GuiButton(startButton, "Symulacja"))
            {
                
                v = 0.0f;
                a = 0.0f;
                amp = amp_0 - 40.0f;
                czas.clear();
                wychylenie.clear();
                przysp.clear();
                przysp.push_back(a);

                currSt = SIMULATION;
                SetWindowTitle("Sprezynka - Symulacja");
            }

            EndDrawing();
        }
        else if (currSt == SIMULATION) {

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
            ClearBackground(WHITE);

            GuiLabel(Rectangle{ 20, 20, 200, 20 }, "Wytlumienie c");

            GuiSlider(Rectangle{ 20, 45, 200, 20 }, TextFormat("%.2f", c_min), TextFormat("%.2f", c_max),
                &c, c_min, c_max);

            //DrawLine(0, (screenHeight / 2), screenWidth, (screenHeight / 2), RED);
            //DrawLine((screenWidth / 2), 0, (screenWidth / 2), screenHeight, RED);

            DrawLine((screenWidth / 2), amp, (screenWidth / 2), temp, BLACK);

            DrawRectangle(x_0, amp, recWidth, recHeight, GRAY);

            Sinusoida(wychylenie, plotx, ploty, plotw, ploth);



            cout << "a: " << a << ", v: " << v << ", wychylenie: " << amp_0 - amp << endl;


            EndDrawing();
        }
    }

    CloseWindow();

    std::vector<float> analityczne;

    float beta = c / (2.0f * m);
    float w0 = sqrt(k / m);
    float wd = sqrt(w0 * w0 - beta * beta);

    float A = 40.0f;
    float phi = 0.0f;

    for (int i = 0; i < czas.size(); i++)
    {
        float t = czas[i];
        analityczne.push_back(
            A * exp(-beta * t) * cos(wd * t + phi)
        );
    }

    Wykres(czas, analityczne, wychylenie, "Czas [s]", "Wychylenie [cm]", "Wykres Wychylenia od czasu");
    //WykPrzys(czas, przysp);

    return 0;
}