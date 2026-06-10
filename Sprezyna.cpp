#include "raylib.h"
#include "Constants.h"
#include "SimModel.h"
#include "SimData.h"
#include "Renderer.h"
#include "charts.h"

#include <iostream>
#include <cmath>

#define RAYGUI_IMPLEMENTATION
extern "C" {
#include "raygui.h"
}

enum SimState {
    MENU,
    SETTINGS,
    SIM2D,
    SIM3D,
    DUAL
};

//skalowanie symulacji 3D na podstawie 2D
static inline float toWorld(float pixAmp, float equiPx) {
    return Consts::c3d_anchorY - (pixAmp - equiPx) * Consts::scale3d;
}

using namespace std;

int main() {

    InitWindow(Consts::screenWidth, Consts::screenHeight, "Sprezynka");
    SetTargetFPS(Consts::FPS);

    SimState current = MENU;

    //STAŁE

    //wspólne
    float start_k = Consts::k;
    float start_m = Consts::m;
    float start_c = Consts::c;
    float start_offset = 40.0f;
    float start_offset_x = 30.0f; //3D
    float start_offset_z = 18.0f; //3D

    const float k_min = 50.0f, k_max = 1000.0f;
    const float m_min = Consts::m_min, m_max = Consts::m_max;
    const float c_min = Consts::c_min, c_max = Consts::c_max;
    const float off_min = -100.0f, off_max = 100.0f;

    //dla symulacji 2D
    float damping2d = start_c;
    float mass2d = start_m;
    float init_disp = start_offset;
    auto model2d = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);
    auto data2d = SimData();
    float sim_Time2d = 0.0f;
    float accum2d = 0.0f;

    //dla symulacji 3D
    float damping3d = start_c;
    float mass3d = start_m;
    auto model3d = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);
    float accum3d = 0.0f;

    //kamera do 3D
    Camera3D cam3d = { 0 };
    cam3d.position = { Consts::c3d_posX, Consts::c3d_posY, Consts::c3d_posZ };
    cam3d.target = { 0.0f, Consts::c3d_anchorY - 2.0f, 0.0f };
    cam3d.up = { 0.0f, 1.0f, 0.0f };
    cam3d.fovy = Consts::c3d_fovY;
    cam3d.projection = CAMERA_PERSPECTIVE; //można zmieniać orientacje kamery

    //symulacja podwójna
    const float k_ref = 500.0f, m_ref = 5.0f, c_ref = 0.5f;
    auto modelRef = SimModel(k_ref, m_ref, c_ref, Consts::screenHeight / 2.0f);
    auto modelUser = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);
    modelRef.reset(Consts::screenHeight / 2.0f - 40.0f);
    modelUser.reset(Consts::screenHeight / 2.0f - start_offset);
    float dual_accumulator = 0.0f;

    float btnX = Consts::screenWidth / 2.0f;
    float btnY = Consts::screenHeight / 2.0f;
    float btnW = Consts::screenWidth / 4.0f;
    float btnH = 65.0f;

    bool graph = false;

    while (!WindowShouldClose()) {

        if (current == MENU) { //MENU
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("Symulacja sprezyny", (btnX - btnW / 2.0f) - 140, 100, 60, DARKGRAY);

            if (GuiButton(Rectangle{ btnX - btnW / 2.0f, btnY - 200, btnW, btnH }, "Symulacja 2D")) {
                //reset dla 2D
                damping2d = start_c;
                mass2d = start_m;
                init_disp = start_offset;
                model2d = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);
                model2d.reset(Consts::screenHeight / 2.0f - start_offset);
                data2d.clear();
                sim_Time2d = 0.0f;
                accum2d = 0.0f;
                current = SIM2D;
                SetWindowTitle("Sprezynka - symulacja 2D");
            }

            if (GuiButton(Rectangle{ btnX - btnW / 2.0f, btnY - 100, btnW, btnH }, "Symulacja 3D")) {
                //reset dla 3D
                damping3d = start_c;
                mass3d = start_m;
                model3d = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);
                model3d.reset(Consts::screenHeight / 2.0f - start_offset);
                model3d.resetX(start_offset_x);
                model3d.resetZ(start_offset_z);
                model3d.setSpringConstantX(start_k * 0.15f);
                model3d.setSpringConstantZ(start_k * 0.15f);
                accum3d = 0.0f;
                cam3d.position = { Consts::c3d_posX, Consts::c3d_posY, Consts::c3d_posZ };
                cam3d.target = { 0.0f, Consts::c3d_anchorY - 2.0f, 0.0f };
                current = SIM3D;
                SetWindowTitle("Sprezynka - symulacja 3D");
            }

            if (GuiButton(Rectangle{ btnX - btnW / 2.0f, btnY, btnW, btnH }, "Symulacja podwojna")) {
                modelRef = SimModel(k_ref, m_ref, c_ref, Consts::screenHeight / 2.0f);
                modelUser = SimModel(start_k, start_m, start_c, Consts::screenHeight / 2.0f);
                modelRef.reset(Consts::screenHeight / 2.0f - 40.0f);
                modelUser.reset(Consts::screenHeight / 2.0f - start_offset);
                dual_accumulator = 0.0f;
                current = DUAL;
                SetWindowTitle("Sprezynka - symulacja podwojna");
            }

            if (GuiButton(Rectangle{ btnX - btnW / 2.0f, btnY + 100, btnW, btnH }, "Ustawienia")) {
                current = SETTINGS;
            }

            DrawText(TextFormat("k=%.1f  m=%.2f  c=%.2f  y=%.1f  x=%.1f  z=%.1f",
                start_k, start_m, start_c, start_offset, start_offset_x, start_offset_z),
                (btnX - btnW / 2.0f) - 100, btnY + 200, 20, DARKGRAY);

            EndDrawing();
        }

        else if (current == SETTINGS) { //SETTINGS
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Ustawienia", btnX - 120, 50, 50, DARKGRAY);
            DrawText("(parametry wspolne dla 2D i 3D)", btnX - 120, 105, 18, GRAY);

            GuiLabel(Rectangle{ btnX - btnW / 2.0f, btnY - 320, 250, 20 }, "Stala sprezystosci k [N/m]");
            GuiSlider(Rectangle{ btnX - btnW / 2.0f, btnY - 300, 300, 20 },
                TextFormat("%.1f", k_min), TextFormat("%.1f", k_max), &start_k, k_min, k_max);
            DrawText(TextFormat("%.1f", start_k), (btnX - btnW / 2.0f) + 340, btnY - 300, 20, DARKGRAY);

            GuiLabel(Rectangle{ btnX - btnW / 2.0f, btnY - 270, 250, 20 }, "Masa m [kg]");
            GuiSlider(Rectangle{ btnX - btnW / 2.0f, btnY - 250, 300, 20 },
                TextFormat("%.2f", m_min), TextFormat("%.2f", m_max), &start_m, m_min, m_max);
            DrawText(TextFormat("%.2f", start_m), (btnX - btnW / 2.0f) + 340, btnY - 250, 20, DARKGRAY);

            GuiLabel(Rectangle{ btnX - btnW / 2.0f, btnY - 220, 250, 20 }, "Tlumienie c [Ns/m]");
            GuiSlider(Rectangle{ btnX - btnW / 2.0f, btnY - 200, 300, 20 },
                TextFormat("%.2f", c_min), TextFormat("%.2f", c_max), &start_c, c_min, c_max);
            DrawText(TextFormat("%.2f", start_c), (btnX - btnW / 2.0f) + 340, btnY - 200, 20, DARKGRAY);

            GuiLabel(Rectangle{ btnX - btnW / 2.0f, btnY - 170, 250, 20 }, "Poczatkowe wychylenie y [px]");
            GuiSlider(Rectangle{ btnX - btnW / 2.0f, btnY - 150, 300, 20 },
                TextFormat("%.1f", off_min), TextFormat("%.1f", off_max), &start_offset, off_min, off_max);
            DrawText(TextFormat("%.1f", start_offset), (btnX - btnW / 2.0f) + 340, btnY - 150, 20, DARKGRAY);

            GuiLabel(Rectangle{ btnX - btnW / 2.0f, btnY - 120, 250, 20 }, "Poczatkowe wychylenie x [px] (3D)");
            GuiSlider(Rectangle{ btnX - btnW / 2.0f, btnY - 100, 300, 20 },
                TextFormat("%.1f", off_min), TextFormat("%.1f", off_max), &start_offset_x, off_min, off_max);
            DrawText(TextFormat("%.1f", start_offset_x), (btnX - btnW / 2.0f) + 340, btnY - 100, 20, DARKGRAY);

            GuiLabel(Rectangle{ btnX - btnW / 2.0f, btnY - 70, 250, 20 }, "Poczatkowe wychylenie z [px] (3D)");
            GuiSlider(Rectangle{ btnX - btnW / 2.0f, btnY - 50, 300, 20 },
                TextFormat("%.1f", off_min), TextFormat("%.1f", off_max), &start_offset_z, off_min, off_max);
            DrawText(TextFormat("%.1f", start_offset_z), (btnX - btnW / 2.0f) + 340, btnY - 50, 20, DARKGRAY);

            if (GuiButton(Rectangle{ Consts::screenWidth / 2.0f - 100, 480, 200, 50 }, "Powrot do menu")) {
                current = MENU;
            }
            EndDrawing();
        }

        else if (current == SIM2D) { //SIM 2D
            float frame_t = GetFrameTime();
            accum2d += frame_t;
            model2d.setMass(mass2d);

            while (accum2d >= Consts::DT) {
                model2d.setDamping(damping2d);
                model2d.update(Consts::DT);
                sim_Time2d += Consts::DT;
                data2d.record(sim_Time2d, model2d.getDisplacement(), model2d.getAcceleration());
                accum2d -= Consts::DT;
            }

            BeginDrawing();
            ClearBackground(GRAY);

            DrawRectangle(0, 0, 400, 150, LIGHTGRAY);

            GuiLabel(Rectangle{ 25, 20, 200, 20 }, "Wytlumienie c");
            GuiSlider(Rectangle{ 25, 45, 200, 20 },
                TextFormat("%.2f", c_min), TextFormat("%.2f", c_max), &damping2d, c_min, c_max);

            GuiLabel(Rectangle{ 25, 65, 200, 20 }, "Masa m");
            GuiSlider(Rectangle{ 25, 90, 200, 20 },
                TextFormat("%.2f", m_min), TextFormat("%.2f", m_max), &mass2d, m_min, m_max);

            GuiCheckBox(Rectangle{ 25, 118, 16, 16 }, "Pokaz wykres po symulacji", & graph);

            if (GuiButton(Rectangle{ Consts::screenWidth - 120.0f, 20, 100, 30 }, "Menu")) {
                current = MENU;
                SetWindowTitle("Sprezynka");
            }

            float amp = model2d.getAmplitude();
            float anchorX = Consts::screenWidth / 2.0f;

            Renderer::drawSpring(anchorX, anchorX, Consts::p_spring, amp, 8.0f);
            Renderer::drawBox2D(anchorX, amp, Consts::recWidth, Consts::recHeight, RAYWHITE);

            const auto& disp = data2d.getDisplacement();
            float center_Y = Consts::screenHeight / 2.0f + Consts::recHeight / 2.0f;
            float plot_Y = center_Y - Consts::plotHeight / 2.0f;
            Renderer::drawScatterPlot(disp, 0, static_cast<int>(plot_Y),
                Consts::plotWidth, Consts::plotHeight, center_Y, 4, DARKBLUE);

            DrawText("2D", 10, Consts::screenHeight - 25, 20, WHITE);

            EndDrawing();
        }
        else if (current == SIM3D) { //SIM3D
            float frame_t = GetFrameTime();
            accum3d += frame_t;
            model3d.setMass(mass3d);

            while (accum3d >= Consts::DT) {
                model3d.setDamping(damping3d);
                model3d.update(Consts::DT);
                accum3d -= Consts::DT;
            }

            UpdateCamera(&cam3d, CAMERA_THIRD_PERSON);

            float equiPx = model3d.getEquilibrium();
            float worldY = toWorld(model3d.getAmplitude(), equiPx); //tu jest wywoływane skalowanie
            float worldX = model3d.getX() * Consts::scale3d;
            float worldZ = model3d.getZ() * Consts::scale3d;

            Vector3 anchorPos = { 0.0f, 10.0f, 0.0f };
            Vector3 massPos = { worldX, worldY, worldZ };

            BeginDrawing();
            ClearBackground(DARKGRAY);

            BeginMode3D(cam3d);

            DrawGrid(16, 1.0f);

            DrawCube(anchorPos, 1.5f, 0.15f, 1.5f, LIGHTGRAY);
            DrawCubeWires(anchorPos, 1.5f, 0.15f, 1.5f, DARKGRAY);

            Renderer::drawSpring3D(anchorPos, massPos, 20, 0.3f, BLACK);

            Renderer::drawMass3D(massPos, 0.8f, RAYWHITE);

            for (int i = 0; i < 8; ++i) {
                if (i % 2 == 0) {
                    float y0 = Consts::c3d_anchorY - i * 0.4f;
                    float y1 = Consts::c3d_anchorY - (i + 1) * 0.4f;
                    DrawLine3D({ 0, y0, 0 }, { 0, y1, 0 }, GRAY);
                }
            }

            EndMode3D();
            
            //interfejs
            DrawRectangle(0, 0, 250, 150, Fade(BLACK, 0.45f));

            GuiLabel(Rectangle{ 25, 10, 180, 20 }, "Wytlumienie c");
            GuiSlider(Rectangle{ 25, 30, 200, 16 },
                TextFormat("%.2f", c_min), TextFormat("%.2f", c_max), &damping3d, c_min, c_max);

            GuiLabel(Rectangle{ 25, 52, 180, 20 }, "Masa m");
            GuiSlider(Rectangle{ 25, 72, 200, 16 },
                TextFormat("%.2f", m_min), TextFormat("%.2f", m_max), &mass3d, m_min, m_max);

            GuiCheckBox(Rectangle{ 20, 118, 16, 16 }, "Pokaz wykres po symulacji", & graph);

            DrawText(TextFormat("k=%.0f  m=%.2f  c=%.2f", start_k, mass3d, damping3d),
                10, 96, 14, LIGHTGRAY);

            if (GuiButton(Rectangle{ Consts::screenWidth - 120.0f, 10, 100, 30 }, "Menu")) {
                current = MENU;
                SetWindowTitle("Sprezynka");
            }

            EndDrawing();
        }

        else if (current == DUAL) { //podwójna symulacja
            float frame_t = GetFrameTime();
            dual_accumulator += frame_t;

            while (dual_accumulator >= Consts::DT) {
                modelRef.update(Consts::DT);
                modelUser.setDamping(start_c);
                modelUser.setMass(start_m);
                modelUser.setSpringConstant(start_k);
                modelUser.update(Consts::DT);
                dual_accumulator -= Consts::DT;
            }

            BeginDrawing();
            ClearBackground(GRAY);

            DrawRectangle(380, 0, 400, 150, LIGHTGRAY);
            GuiLabel(Rectangle{ 420, 10, 200, 20 }, "Parametry modelu uzytkownika");

            GuiLabel(Rectangle{ 420, 35, 150, 20 }, "k [N/m]");
            GuiSlider(Rectangle{ 420, 55, 300, 10 },
                TextFormat("%.1f", k_min), TextFormat("%.1f", k_max), &start_k, k_min, k_max);

            GuiLabel(Rectangle{ 420, 65, 150, 20 }, "m [kg]");
            GuiSlider(Rectangle{ 420, 85, 300, 10 },
                TextFormat("%.2f", m_min), TextFormat("%.2f", m_max), &start_m, m_min, m_max);

            GuiLabel(Rectangle{ 420, 100, 150, 20 }, "c [Ns/m]");
            GuiSlider(Rectangle{ 420, 125, 300, 10 },
                TextFormat("%.2f", c_min), TextFormat("%.2f", c_max), &start_c, c_min, c_max);

            if (GuiButton(Rectangle{ Consts::screenWidth - 120.0f, 20, 100, 30 }, "Menu")) {
                current = MENU;
                SetWindowTitle("Sprezynka");
            }

            //blok przykładowy
            float ampRef = modelRef.getAmplitude();
            float x0Ref = Consts::screenWidth / 4.0f - Consts::recWidth / 2.0f;
            float springXRef = x0Ref + Consts::recWidth / 2.0f;
            Renderer::drawSpring(springXRef, springXRef, Consts::p_spring, ampRef, 8.0f);
            Renderer::drawBox(x0Ref, ampRef, Consts::recWidth, Consts::recHeight, SKYBLUE);
            DrawText("Model referencyjny", (int)x0Ref, 150, 16, DARKBLUE);

            //blok użytkownika
            float ampUser = modelUser.getAmplitude();
            float x0User = 3 * Consts::screenWidth / 4.0f - Consts::recWidth / 2.0f;
            float springXUser = x0User + Consts::recWidth / 2.0f;
            Renderer::drawSpring(springXUser, springXUser, Consts::p_spring, ampUser, 8.0f);
            Renderer::drawBox(x0User, ampUser, Consts::recWidth, Consts::recHeight, DARKGREEN);
            DrawText("Model uzytkownika", (int)x0User, 150, 16, DARKGREEN);

            DrawText(TextFormat("k=%.1f  m=%.2f  c=%.2f", start_k, start_m, start_c),
                420, Consts::screenHeight - 40, 16, BLACK);

            EndDrawing();
        }
    }

    CloseWindow();

    if (!data2d.getTime().empty()) {
        float beta = damping2d / (2.0f * mass2d);
        float w0 = std::sqrt(start_k / mass2d);
        float wd = std::sqrt(std::max(0.0f, w0 * w0 - beta * beta));
        float A = init_disp;

        std::vector<float> analityczne;
        for (float t : data2d.getTime()) {
            analityczne.push_back(A * std::exp(-beta * t) * std::cos(wd * t));
        }

        if (graph == true) {
            Wykres(data2d.getTime(), analityczne, data2d.getDisplacement(),
                "Czas [s]", "Wychylenie [cm]", "Wykres Wychylenia od czasu");
         }
         
    }

    return 0;
}