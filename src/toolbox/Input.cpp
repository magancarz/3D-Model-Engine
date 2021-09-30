#include "Input.h"

Input::Input() {
	for(int i = 0; i < NUM_KEYS; i++) {
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
		std::cerr << "Warning: tried to set key code bigger than NUM_KEYS value.\n";
		exit(1);
	}
	keyDown[keyCode] = value;
}