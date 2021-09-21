#pragma once

#include "../Headers.h"
#include "../toolbox/Input.h"

const unsigned int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 800;
const std::string WINDOW_TITLE = "3D Model Viewer";

extern bool isCloseRequested;
extern Input inputManager;

class DisplayManager {
public:
	void createDisplay();
	void updateDisplay();
	void closeDisplay();

	void checkCloseRequests();

	inline GLFWwindow* getWindow() { return window; };
private:
	GLFWwindow* window;
};