#pragma once

#include "../Headers.h"

class Input {
public:
	Input();

	void setKeyDown(unsigned int keyCode, bool value);
	bool isKeyDown(unsigned int keyCode);

private:
	static constexpr int NUM_KEYS = 512;
	bool keyDown[NUM_KEYS];
};