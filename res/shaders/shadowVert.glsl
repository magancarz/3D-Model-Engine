#version 460 core

in vec3 in_position;

uniform mat4 mvp;

void main(void) {
	gl_Position = mvp * vec4(in_position, 1.0);
}