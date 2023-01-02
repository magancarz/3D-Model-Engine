#include "DisplayManager.h"

#include <iostream>
#include <stdexcept>

#include "Input.h"

void DisplayManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			Input::setKeyDown(key, true);
			break;
		default:
			Input::setKeyDown(key, true);
			break;
		}
	} else if(action == GLFW_RELEASE) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			Input::setKeyDown(key, false);
			break;
		default:
			Input::setKeyDown(key, false);
			break;
		}
	}
}

void DisplayManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if(action == GLFW_PRESS) {
		if(button == GLFW_MOUSE_BUTTON_LEFT)
			Input::setLeftMouseButtonDown(true);
		if(button == GLFW_MOUSE_BUTTON_RIGHT)
			Input::setRightMouseButtonDown(true);
	} else if(action == GLFW_RELEASE) {
		if(button == GLFW_MOUSE_BUTTON_LEFT)
			Input::setLeftMouseButtonDown(false);
		if(button == GLFW_MOUSE_BUTTON_RIGHT)
			Input::setRightMouseButtonDown(false);
	}
}

void DisplayManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	if(firstMouse) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	//Calculate offset
	mouseOffsetX = xpos - lastMouseX;
	mouseOffsetY = ypos - lastMouseY;

	//Set last X and Y
	lastMouseX = xpos;
	lastMouseY = ypos;
}

void DisplayManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	mouseWheel += yoffset;
}

void DisplayManager::create_display() {
	//Create GLFW window and gl context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if(!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window!");
    }

    glfwMakeContextCurrent(m_window);

	glEnable(GL_MULTISAMPLE);

	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
	glfwSetCursorPosCallback(m_window, cursorPosCallback);
	glfwSetScrollCallback(m_window, scrollCallback);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Init GLEW after creating gl context
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		std::cout << "[GLEW] failed to initialize GLEW!" << std::endl;

	m_lastFrameTime = getCurrentTime();
}

void DisplayManager::updateDisplay() {
	glfwSwapBuffers(m_window);
	long currentFrameTime = getCurrentTime();
	m_delta = (currentFrameTime - m_lastFrameTime) / 1000.0f;
	m_lastFrameTime = currentFrameTime;
}

void DisplayManager::closeDisplay() {
	glfwTerminate();
}

void DisplayManager::checkCloseRequests() {
	isCloseRequested = glfwWindowShouldClose(m_window);
}

void DisplayManager::resetInputValues() {
	mouseOffsetX = 0;
	mouseOffsetY = 0;
	mouseWheel = 0;
}

long DisplayManager::getCurrentTime() {
	return glfwGetTime() * 1000;
}

float DisplayManager::getFrameTimeSeconds() {
	return m_delta;
}