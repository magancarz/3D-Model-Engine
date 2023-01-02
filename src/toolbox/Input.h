#pragma once

class Input {
public:
	static void initialize_input();

	static void setKeyDown(unsigned int keyCode, bool value);
	static bool isKeyDown(unsigned int keyCode);

	static void setLeftMouseButtonDown(bool value) { leftMouseButtonDown = value; };
	static void setRightMouseButtonDown(bool value) { rightMouseButtonDown = value; };
	static bool isLeftMouseButtonDown() { return leftMouseButtonDown; };
	static bool isRightMouseButtonDown() { return rightMouseButtonDown; };

private:
	inline static constexpr int NUM_KEYS = 512;
	inline static bool keyDown[NUM_KEYS];

	inline static bool leftMouseButtonDown = false,
				rightMouseButtonDown = false;
};