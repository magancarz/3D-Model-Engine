#include "DisplayManager.h"

#include <iostream>
#include <stdexcept>

#include "Input.h"

void DisplayManager::key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods) {
	if(action == GLFW_PRESS) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			Input::set_key_down(key, true);
			break;
		default:
			Input::set_key_down(key, true);
			break;
		}
	} else if(action == GLFW_RELEASE) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			Input::set_key_down(key, true);
			break;
		default:
			Input::set_key_down(key, false);
			break;
		}
	}
}

void DisplayManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(action == GLFW_PRESS) {
		switch(button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			Input::set_left_mouse_button_down(true);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			Input::set_right_mouse_button_down(true);
			break;
		}
	} else if(action == GLFW_RELEASE) {
		switch(button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			Input::set_left_mouse_button_down(false);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			Input::set_right_mouse_button_down(false);
			break;
		}
	}
}

void DisplayManager::cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos) {
	if(first_mouse) {
		last_mouse_x = x_pos;
		last_mouse_y = y_pos;
		first_mouse = false;
	}

	//Calculate offset
	mouse_offset_x = x_pos - last_mouse_x;
	mouse_offset_y = y_pos - last_mouse_y;

	//Set last X and Y
	last_mouse_x = x_pos;
	last_mouse_y = y_pos;
}

void DisplayManager::scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
	mouse_wheel += y_offset;
}

void DisplayManager::create_display() {
	//Create GLFW window and gl context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window!");
    }

    glfwMakeContextCurrent(window);

	glEnable(GL_MULTISAMPLE);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Init GLEW after creating gl context
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		std::cout << "[GLEW] failed to initialize GLEW!" << std::endl;

	last_frame_time = get_current_time();
}

void DisplayManager::update_display() {
	glfwSwapBuffers(window);
	const long current_frame_time = get_current_time();
	delta = static_cast<double>(current_frame_time - last_frame_time) / 1000.0;
	last_frame_time = current_frame_time;
}

void DisplayManager::close_display() {
	glfwTerminate();
}

void DisplayManager::check_close_requests() {
	is_close_requested = glfwWindowShouldClose(window);
}

void DisplayManager::reset_input_values() {
	mouse_offset_x = 0;
	mouse_offset_y = 0;
	mouse_wheel = 0;
}

long DisplayManager::get_current_time() {
	return static_cast<long>(glfwGetTime() * 1000.0);
}

double DisplayManager::get_frame_time_seconds() {
	return delta;
}

double DisplayManager::get_mouse_x() {
	return last_mouse_x;
}

double DisplayManager::get_mouse_y() {
	return last_mouse_y;
}

double DisplayManager::get_mouse_x_offset() {
	return mouse_offset_x;
}

double DisplayManager::get_mouse_y_offset() {
	return mouse_offset_y;
}

double DisplayManager::get_d_wheel() {
	return mouse_wheel;
}

GLFWwindow* DisplayManager::get_window() {
	return window;
}