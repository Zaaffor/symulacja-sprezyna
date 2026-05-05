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
    SIMULATION,
    SETTINGS
};

using namespace std;
int main() {

    InitWindow(Consts::screenWidth, Consts::screenHeight, "Sprezynka");
    SetTargetFPS(Consts::FPS);

    SimState current = MENU;

    float start_k = Consts::k;
    float start_m = Consts::m;
    float start_c = Consts::c;
    float start_offset = 40.0f;
    float init_disp = start_offset;

    //zakresy suwaków
    const float k_min = 50.0f, k_max = 1000.f;
    const float m_min = Consts::m_min, m_max = Consts::m_max;
    const float c_min = Consts::c_min, c_max = Consts::c_max;
    const float off_min = -100.f, off_max = 100.0f;

    float damping = start_c;
    float m = start_m;

    auto model = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);
    auto data = SimData();
    float sim_Time = 0.0f;
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        if (current == MENU) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Symulacja sprezyny", 200, 150, 40, DARKGRAY);

            Rectangle startBtn = { Consts::screenWidth / 2.0f - 100, 300, 200, 50 };
            if (GuiButton(startBtn, "Symulacja")) {
                damping = start_c;
                m = start_m;
                init_disp = start_offset;

                model = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);

                float start_amp = Consts::screenHeight / 2.0f - start_offset;
                model.reset(start_amp);
                data.clear();
                sim_Time = 0.0f;
                accumulator = 0.0f;
                current = SIMULATION;
                SetWindowTitle("Sprezynka - symulacja");
            }
            Rectangle settBtn = { Consts::screenWidth / 2.0f - 100, 350, 200, 50 };
            if (GuiButton(settBtn, "Ustawienia")) {
                current = SETTINGS;
            }
            EndDrawing();
        }
        else if (current == SETTINGS) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Ustawienia", 200, 50, 40, DARKGRAY);

            // Suwaki
            GuiLabel(Rectangle{ 150, 120, 200, 20 }, "Stala sprezystosci k [N/m]");
            GuiSlider(Rectangle{ 150, 150, 300, 20 }, TextFormat("%.1f", k_min), TextFormat("%.1f", k_max),
                &start_k, k_min, k_max);
            DrawText(TextFormat("%.1f", start_k), 460, 145, 20, DARKGRAY);

            GuiLabel(Rectangle{ 150, 180, 200, 20 }, "Masa m [kg]");
            GuiSlider(Rectangle{ 150, 210, 300, 20 }, TextFormat("%.2f", m_min), TextFormat("%.2f", m_max),
                &start_m, m_min, m_max);
            DrawText(TextFormat("%.2f", start_m), 460, 205, 20, DARKGRAY);

            GuiLabel(Rectangle{ 150, 240, 200, 20 }, "Tlumienie c [Ns/m]");
            GuiSlider(Rectangle{ 150, 270, 300, 20 }, TextFormat("%.2f", c_min), TextFormat("%.2f", c_max),
                &start_c, c_min, c_max);
            DrawText(TextFormat("%.2f", start_c), 460, 265, 20, DARKGRAY);

            GuiLabel(Rectangle{ 150, 300, 250, 20 }, "Poczatkowe wychylenie [px]");
            GuiSlider(Rectangle{ 150, 330, 300, 20 }, TextFormat("%.1f", off_min), TextFormat("%.1f", off_max),
                &start_offset, off_min, off_max);
            DrawText(TextFormat("%.1f", start_offset), 460, 325, 20, DARKGRAY);

            Rectangle backBtn = { Consts::screenWidth / 2.0f - 100, 450, 200, 50 };
            if (GuiButton(backBtn, "Powrot do menu")) {
                current = MENU;
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
            GuiSlider(Rectangle{ 20, 45, 200, 20 }, TextFormat("%.2f", c_min), TextFormat("%.2f", 
                c_max), &damping, c_min, c_max);

            GuiLabel(Rectangle{ 20, 60, 200, 20 }, "Masa m");
            GuiSlider(Rectangle{ 20, 85, 200, 20 }, TextFormat("%.2f", m_min), TextFormat("%.2f",
                m_max), &m, m_min, m_max);

            Rectangle menuBtn = { Consts::screenWidth - 120, 20, 100, 30 };
            if (GuiButton(menuBtn, "Menu")) {
                current = MENU;
            }

            //rysowanie sprężyny i klocka
            float amp = model.getAmplitude();
            float x_0 = Consts::screenWidth / 2.0f - Consts::recWidth / 2.0f;
            float spring_X = x_0 + Consts::recWidth / 2.0f;
            Renderer::drawSpring(spring_X, Consts::p_spring, amp, 8.0f);
            Renderer::drawBox(x_0, amp, Consts::recWidth, Consts::recHeight, RAYWHITE);

            const auto& disp = data.getDisplacement();
            float center_Y = Consts::screenHeight / 2.0f + Consts::recHeight / 2.0f;
            float plot_Y = center_Y - Consts::plotHeight / 2.0f;
            Renderer::drawScatterPlot(disp, 0, static_cast<int>(plot_Y), Consts::plotWidth,
                Consts::plotHeight, center_Y, 4, DARKBLUE);

            EndDrawing();
        }
    }

    CloseWindow();

    if (!data.getTime().empty()) {
        float beta = damping / (2.0f * m);
        float w0 = std::sqrt(start_k / m);
        float wd = std::sqrt(w0 * w0 - beta * beta);
        float A = init_disp;
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