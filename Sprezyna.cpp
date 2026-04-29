#include "raylib.h"
#include "raygui.h"
#include "Constants.h"
#include "SimModel.h"
#include "SimData.h"
#include "Renderer.h"
#include "charts.h"      

#include <iostream>

#define RAYGUI_IMPLEMENTATION
extern "C" {
    #include "raygui.h"
}

enum SimState {
    MENU,
    SIMULATION
};

using namespace std;
int main() {

    InitWindow(Consts::screenWidth, Consts::screenHeight, "Sprezynka");
    SetTargetFPS(Consts::FPS);

    SimState current = MENU;

    float damping = Consts::c;
    float m = Consts::m;
    auto model = SimModel(Consts::k, Consts::m, damping, Consts::screenHeight / 2.0f);
    auto data = SimData();
    float sim_Time = 0.0f;
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        if (current == MENU) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Symulacja sprezyny", 200, 150, 40, DARKGRAY);

            Rectangle start = { Consts::screenWidth / 2.0f - 100, 300, 200, 50 };
            if (GuiButton(start, "Symulacja")) {
                damping = Consts::c;
                model = SimModel(Consts::k, m, damping, Consts::screenHeight / 2.0f);
                model.reset(Consts::screenHeight / 2.0f - 40.0f);
                data.clear();
                sim_Time = 0.0f;
                accumulator = 0.0f;
                current = SIMULATION;
                SetWindowTitle("Sprezynka - symulacja");
            }
            EndDrawing();
        }
        else if (current == SIMULATION) {
            float frame_t = GetFrameTime();
            accumulator += frame_t;
            model.setMass(m);

            while (accumulator >= Consts::DT) {
                model.setDamping(damping);
                model.update(Consts::DT);
                sim_Time += Consts::DT;
                data.record(sim_Time, model.getDisplacement(), model.getAcceleration());
                accumulator -= Consts::DT;
            }

            BeginDrawing();
            ClearBackground(GRAY);

            GuiLabel(Rectangle{ 20, 20, 200, 20 }, "Wytlumienie c");
            GuiSlider(Rectangle{ 20, 45, 200, 20 }, TextFormat("%.2f", Consts::c_min), TextFormat("%.2f", 
                Consts::c_max), &damping, Consts::c_min, Consts::c_max);

            GuiLabel(Rectangle{ 20, 60, 200, 20 }, "Masa m");
            GuiSlider(Rectangle{ 20, 85, 200, 20 }, TextFormat("%.2f", Consts::m_min), TextFormat("%.2f",
                Consts::m_max), &m, Consts::m_min, Consts::m_max);

            float amp = model.getAmplitude();
            float x_0 = Consts::screenWidth / 2.0f - Consts::recWidth / 2.0f;
            float spring_Y = amp - 30.0f;
            float spring_X = x_0 + Consts::recWidth / 2.0f;
            Renderer::drawSpring(spring_X, Consts::p_spring, amp, 8.0f);
            Renderer::drawBox(x_0, amp, Consts::recWidth, Consts::recHeight, RAYWHITE);

            const auto& disp = data.getDisplacement();
            float center_Y = Consts::screenHeight / 2.0f;
            float plot_Y = center_Y - Consts::plotHeight / 2.0f;
            Renderer::drawScatterPlot(disp, 0, static_cast<int>(plot_Y), Consts::plotWidth,
                Consts::plotHeight, center_Y, 4, DARKBLUE);

            EndDrawing();
        }
    }

    CloseWindow();

    if (!data.getTime().empty()) {
        float beta = damping / (2.0f * Consts::m);
        float w0 = std::sqrt(Consts::k / Consts::m);
        float wd = std::sqrt(w0 * w0 - beta * beta);
        float A = 40.0f;
        float phi = 0.0f;

        std::vector<float> analityczne;
        for (float t : data.getTime()) {
            analityczne.push_back(A * std::exp(-beta * t) * std::cos(wd * t + phi));
        }

        Wykres(data.getTime(), analityczne, data.getDisplacement(),
            "Czas [s]", "Wychylenie [cm]", "Wykres Wychylenia od czasu");
    }

    return 0;
}