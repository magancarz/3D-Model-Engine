#pragma once

#include "../Headers.h"
#include "../toolbox/Input.h"
#include "../entities/Camera.h"

const unsigned int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 800;
const std::string WINDOW_TITLE = "3D Model Viewer";

class DisplayManager {
public:
	void createDisplay();
	void updateDisplay();
	void closeDisplay();

	void checkCloseRequests();

	void rotateCamera(Camera* camera);

	long getCurrentTime();
	float getFrameTimeSeconds();

	inline GLFWwindow* getWindow() { return window; };
private:
	GLFWwindow* window;

	long lastFrameTime;
	float delta;
};

extern DisplayManager display;
extern bool isCloseRequested;
extern Input inputManager;