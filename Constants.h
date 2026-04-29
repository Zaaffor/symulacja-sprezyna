#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Consts {
	//RYSOWANIE
	constexpr int screenWidth = 800;
	constexpr int screenHeight = 600;
	constexpr float recWidth = 100.0f;
	constexpr float recHeight = 50.0f;
	constexpr float FPS = 24.0f;
	constexpr float DT = 1.0f / 120.0f;
	constexpr float p_spring = screenHeight / 2.0f - 150.0f;

	//OBLICZENIA
	constexpr float g = 9.81f;
	constexpr float k = 500.0f;
	constexpr float m = 5.0f;
	constexpr float m_min = 0.5f;
	constexpr float m_max = 15.0f;
	constexpr float c = 0.5f;
	constexpr float c_max = 5.0f;
	constexpr float c_min = 0.0f;

	//RYSOWANIE WYKRESU
	constexpr float amp_off = 40.0f;
	constexpr int plotWidth = screenWidth / 2;
	constexpr int plotHeight = 200;
	constexpr int plotYoffset = 400;
}

#endif
