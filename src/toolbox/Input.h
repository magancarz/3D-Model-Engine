#pragma once

class Input {
public:
	static void initialize_input();

	static void set_key_down(unsigned int key_code, bool value);
	static bool is_key_down(unsigned int key_code);

	static void set_left_mouse_button_down(bool value);
	static void set_right_mouse_button_down(bool value);
	static bool is_left_mouse_button_down();
	static bool is_right_mouse_button_down();

private:
	inline static constexpr int NUM_KEYS = 512;
	inline static bool key_down[NUM_KEYS];

	inline static bool left_mouse_button_down  = false,
					   right_mouse_button_down = false;
};