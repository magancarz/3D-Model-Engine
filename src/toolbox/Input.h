#pragma once

#include "../Headers.h"

class Input {
public:
	Input();

	void setKeyDown(unsigned int keyCode, bool value);
	bool isKeyDown(unsigned int keyCode);

	inline void setLeftMouseButtonDown(bool value) { leftMouseButtonDown = value; };
	inline void setRightMouseButtonDown(bool value) { rightMouseButtonDown = value; };
	inline bool isLeftMouseButtonDown() { return leftMouseButtonDown; };
	inline bool isRightMouseButtonDown() { return rightMouseButtonDown; };

private:
	static constexpr int NUM_KEYS = 512;
	bool keyDown[NUM_KEYS];

	bool leftMouseButtonDown = false,
		 rightMouseButtonDown = false;
};