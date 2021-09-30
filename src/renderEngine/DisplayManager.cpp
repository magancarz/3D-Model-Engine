#include "DisplayManager.h"

//Mouse position
static float mouseOffsetX = 0, mouseOffsetY = 0;
static bool firstMouse = true;
static float lastMouseX = 0, lastMouseY = 0;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			inputManager.setKeyDown(key, true);
			break;
		default:
			inputManager.setKeyDown(key, true);
			break;
		}
	} else if(action == GLFW_RELEASE) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			inputManager.setKeyDown(key, false);
			break;
		default:
			inputManager.setKeyDown(key, false);
			break;
		}
	}
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
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

void DisplayManager::rotateCamera(Camera* camera) {
	camera->rotate(mouseOffsetX, -mouseOffsetY, 0);

	mouseOffsetX = mouseOffsetY = 0;
}

void DisplayManager::createDisplay() {
	//Create GLFW window and gl context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    if(!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window!");
    }

    glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Init GLEW after creating gl context
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		std::cout << "[GLEW] failed to initialize GLEW!" << std::endl;
}

void DisplayManager::updateDisplay() {
	glfwSwapBuffers(window);
}

void DisplayManager::closeDisplay() {
	glfwTerminate();
}

void DisplayManager::checkCloseRequests() {
	isCloseRequested = glfwWindowShouldClose(window);
}