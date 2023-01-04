#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DisplayManager {
public:
	static void create_display();
	static void update_display();
	static void close_display();

	static void check_close_requests();

	static void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos);
	static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

	static void reset_input_values();

	static double get_mouse_x();
	static double get_mouse_y();

	static double get_mouse_x_offset();
	static double get_mouse_y_offset();

	static double get_d_wheel();

	static long get_current_time();
	static double get_frame_time_seconds();

	static GLFWwindow* get_window();

	inline static bool is_close_requested = false;

	inline static const int WINDOW_WIDTH = 1280,
							WINDOW_HEIGHT = 800;

	inline static const char* WINDOW_TITLE = "3D Model Engine";

private:
	inline static GLFWwindow* window;

	inline static long last_frame_time;
	inline static double delta;

	inline static double mouse_offset_x = 0, mouse_offset_y = 0;
	inline static double last_mouse_x = 0, last_mouse_y = 0;
	inline static bool first_mouse = true;

	inline static double mouse_wheel = 0;
};
