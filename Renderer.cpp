#include "Renderer.h"
#include "raylib.h"
#include <algorithm>

void Renderer::drawSpring(float top_X, float bot_X, float f_Y, float m_Y, float width) {
    float topY = f_Y;
    float bottomY = m_Y;
    if (topY > bottomY) std::swap(topY, bottomY);

    float height = bottomY - topY;
    const int segments = 24;
    float stepY = height / segments;
    float amplitude = width * 0.5f;

    float x1 = top_X;
    float y1 = topY;
    bool goRight = true;

    for (int i = 1; i <= segments; ++i) {
        float t = (float)i / segments;                    
        float centerX = top_X + (bot_X - top_X) * t;    
        float y2 = topY + i * stepY;
        float x2 = centerX + (goRight ? amplitude : -amplitude);
        DrawLine(x1, y1, x2, y2, BLACK);
        x1 = x2;
        y1 = y2;
        goRight = !goRight;
    }

    if (y1 != bottomY) {
        DrawLine(x1, y1, bot_X, bottomY, BLACK);
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

void Renderer::drawBox2D(float centerX, float y, float w, float h, Color color) {
    DrawRectangle(static_cast<int>(centerX - w / 2.0f), static_cast<int>(y),
        static_cast<int>(w), static_cast<int>(h), color);
}


void Renderer::drawSpring3D(Vector3 top, Vector3 bot, int seg, float r, Color color) {
    const float endFrac = 0.08f;

    Vector3 springTop = {
        top.x + (bot.x - top.x) * endFrac, top.y + (bot.y - top.y) * endFrac, top.z + (bot.z - top.z) * endFrac
    };
    Vector3 springBottom = {
        top.x + (bot.x - top.x) * (1.0f - endFrac), top.y + (bot.y - top.y) * (1.0f - endFrac), top.z + (bot.z - top.z) * (1.0f - endFrac)
    };

    DrawLine3D(top, springTop, color);
    DrawLine3D(springBottom, bot, color);

    Vector3 prev = springTop;
    bool    goRight = true;

    for (int i = 1; i <= seg; ++i) {
        float t = (float)i / (float)seg;
        Vector3 centre = {
            springTop.x + (springBottom.x - springTop.x) * t,
            springTop.y + (springBottom.y - springTop.y) * t,
            springTop.z + (springBottom.z - springTop.z) * t
        };

        Vector3 p = {
            centre.x + (goRight ? r : -r), centre.y, centre.z
        };
        DrawLine3D(prev, p, color);
        prev = p;
        goRight = !goRight;
    }
    DrawLine3D(prev, springBottom, color);
}

void Renderer::drawMass3D(Vector3 pos, float size, Color color) {
    DrawCube(pos, size, size, size, color);
    DrawCubeWires(pos, size, size, size, BLACK);
}

