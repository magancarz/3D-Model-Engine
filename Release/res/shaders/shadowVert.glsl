#version 460 core

in vec3 in_position;
in vec2 in_textureCoords;

out vec2 textureCoords;

uniform mat4 mvp;

void main(void) {
	gl_Position = mvp * vec4(in_position, 1.0);

	textureCoords = in_textureCoords;
}