#include "Input.h"

#include <ranges>
#include <stdexcept>

void Input::initialize_input() {
	for(const auto i : std::views::iota(0, NUM_KEYS)) {
		keyDown[i] = false;
	}
}

bool Input::isKeyDown(unsigned int keyCode) {
	if(keyCode >= NUM_KEYS)
		return false;
	return keyDown[keyCode];
}

void Input::setKeyDown(unsigned int keyCode, bool value) {
	if(keyCode >= NUM_KEYS) {
		throw std::runtime_error("Warning: tried to set key code bigger than NUM_KEYS value.\n");
	}
	keyDown[keyCode] = value;
}