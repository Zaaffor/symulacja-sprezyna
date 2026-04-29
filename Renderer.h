#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "raylib.h"

class Renderer {
public:
	static void drawSpring(float center_X, float f_Y, float m_Y, float width);
	static void drawBox(float x, float y, float w, float h, Color color);
	static void drawScatterPlot(const std::vector<float>& data, int x, int y, int w, int h,
		float center_Y, int p_spacing, Color color);
};

#endif
