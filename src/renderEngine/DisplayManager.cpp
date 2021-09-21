#include "DisplayManager.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		isCloseRequested = true;
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
