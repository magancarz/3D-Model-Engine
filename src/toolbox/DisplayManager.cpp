#include "DisplayManager.h"

#include <iostream>
#include <stdexcept>

#include "Input.h"

void DisplayManager::key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods) {
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
			glfwSetWindowShouldClose(window, true);
			Input::setKeyDown(key, true);
			break;
		default:
			Input::setKeyDown(key, false);
			break;
		}
	}
}

void DisplayManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(action == GLFW_PRESS) {
		switch(button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			Input::setLeftMouseButtonDown(true);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			Input::setRightMouseButtonDown(true);
			break;
		}
	} else if(action == GLFW_RELEASE) {
		switch(button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			Input::setLeftMouseButtonDown(false);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			Input::setRightMouseButtonDown(false);
			break;
		}
	}
}

void DisplayManager::cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos) {
	if(m_first_mouse) {
		m_last_mouse_x = x_pos;
		m_last_mouse_y = y_pos;
		m_first_mouse = false;
	}

	//Calculate offset
	m_mouse_offset_x = x_pos - m_last_mouse_x;
	m_mouse_offset_y = y_pos - m_last_mouse_y;

	//Set last X and Y
	m_last_mouse_x = x_pos;
	m_last_mouse_y = y_pos;
}

void DisplayManager::scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
	m_mouse_wheel += y_offset;
}

void DisplayManager::create_display() {
	//Create GLFW window and gl context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if(!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window!");
    }

    glfwMakeContextCurrent(m_window);

	glEnable(GL_MULTISAMPLE);

	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetCursorPosCallback(m_window, cursor_pos_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Init GLEW after creating gl context
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		std::cout << "[GLEW] failed to initialize GLEW!" << std::endl;

	m_last_frame_time = get_current_time();
}

void DisplayManager::update_display() {
	glfwSwapBuffers(m_window);
	const long current_frame_time = get_current_time();
	m_delta = static_cast<double>(current_frame_time - m_last_frame_time) / 1000.0;
	m_last_frame_time = current_frame_time;
}

void DisplayManager::close_display() {
	glfwTerminate();
}

void DisplayManager::check_close_requests() {
	is_close_requested = glfwWindowShouldClose(m_window);
}

void DisplayManager::reset_input_values() {
	m_mouse_offset_x = 0;
	m_mouse_offset_y = 0;
	m_mouse_wheel = 0;
}

long DisplayManager::get_current_time() {
	return static_cast<long>(glfwGetTime() * 1000.0);
}

double DisplayManager::get_frame_time_seconds() {
	return m_delta;
}

double DisplayManager::get_mouse_x() { return m_last_mouse_x; }
double DisplayManager::get_mouse_y() { return m_last_mouse_y; }

double DisplayManager::get_mouse_x_offset() { return m_mouse_offset_x; }
double DisplayManager::get_mouse_y_offset() { return m_mouse_offset_y; }

double DisplayManager::get_d_wheel() { return m_mouse_wheel; }

GLFWwindow* DisplayManager::get_window() { return m_window; }