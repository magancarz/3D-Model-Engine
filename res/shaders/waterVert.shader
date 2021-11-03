#version 460 core

in vec2 position;

out vec4 clipSpace;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main(void) {
	clipSpace = proj * view * model * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = clipSpace;
}