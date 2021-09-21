#pragma once

#include "../Headers.h"

class Input {
public:
	Input();

	void setKeyDown(unsigned int keyCode, bool value);
	bool isKeyDown(unsigned int keyCode);
	
	void moveMouse(float dx, float dy);
	inline float getMouseXTotal() { return dxTotal; };
	inline float getMouseYTotal() { return dyTotal; };

private:
	static constexpr int NUM_KEYS = 512;
	bool keyDown[NUM_KEYS];

	float dxTotal, dyTotal;
};