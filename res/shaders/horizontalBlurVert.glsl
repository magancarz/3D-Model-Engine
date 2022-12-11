#version 460 core

in vec2 position;

out vec2 textureSize;
out int direction;

uniform float targetWidth;

void main(void) {
	textureSize = vec2(1280, 800);
	direction = 0;
}