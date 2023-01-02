#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "3D Model Viewer"

class DisplayManager {
public:
	static void create_display();
	static void updateDisplay();
	static void closeDisplay();

	static void checkCloseRequests();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static void resetInputValues();

	static float getMouseX() { return lastMouseX; }
	static float getMouseY() { return lastMouseY; }

	static float getMouseXOffset() { return mouseOffsetX; }
	static float getMouseYOffset() { return mouseOffsetY; }

	static float getDWheel() { return mouseWheel; }

	static long getCurrentTime();
	static float getFrameTimeSeconds();

	static GLFWwindow* getWindow() { return m_window; }

	inline static bool isCloseRequested = false;
private:
	inline static GLFWwindow* m_window;


	inline static long m_lastFrameTime;
	inline static float m_delta;

	inline static float mouseOffsetX = 0.f, mouseOffsetY = 0.f;
	inline static bool firstMouse = true;
	inline static float lastMouseX = 0.f, lastMouseY = 0.f;

	inline static float mouseWheel = 0.f;
};
