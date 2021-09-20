#include "DisplayManager.h"

void DisplayManager::createDisplay() {
	//Create GLFW window and gl context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    if(!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window!");
    }

    glfwMakeContextCurrent(window);

	//Init GLEW after creating gl context
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		std::cout << "[GLEW] failed to initialize GLEW!" << std::endl;
}

void DisplayManager::updateDisplay() {
	glfwSwapBuffers(window);
}

void DisplayManager::closeDisplay() {

}