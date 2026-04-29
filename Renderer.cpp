#include "Renderer.h"
#include "raylib.h"
#include <algorithm>

void Renderer::drawSpring(float center_X, float f_Y, float m_Y, float width){
    float topY = f_Y;
    float bottomY = m_Y;
    if (topY > bottomY) std::swap(topY, bottomY); 

    float height = bottomY - topY;
    const int segments = 24;               
    float stepY = height / segments;
    float amplitude = width * 0.5f;        

    float x1 = center_X;
    float y1 = topY;
    bool goRight = true;

    for (int i = 1; i <= segments; ++i) {
        float y2 = topY + i * stepY;
        float x2 = center_X + (goRight ? amplitude : -amplitude);
        DrawLine(x1, y1, x2, y2, BLACK);
        x1 = x2;
        y1 = y2;
        goRight = !goRight;
    }

    if (y1 != bottomY) {
        DrawLine(x1, y1, center_X, bottomY, BLACK);
    }
}

void Renderer::drawBox(float x, float y, float w, float h, Color color) {
	DrawRectangle(x, y, w, h, color);
}

void Renderer::drawScatterPlot(const std::vector<float>& data, int x, int y, int w, int h,
	float center_Y, int p_spacing, Color color) {

	int count =  static_cast<int>(data.size());
	int max_points = w / p_spacing;
	int start_Idx = std::max(0, count - max_points);
	int visible = count - start_Idx;
	int offset = (max_points - visible) * p_spacing;

	for (int i = start_Idx, idx = 0; i < count; ++i, ++idx) {
		float px = x + offset + idx * p_spacing;
		float py = center_Y - data[i];
		if (py >= y && py <= y + h) {
			DrawCircle(static_cast<int>(px), static_cast<int>(py), 2, color);
		}
	}
}