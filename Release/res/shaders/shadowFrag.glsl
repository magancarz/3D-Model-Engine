#version 460 core

out vec4 out_Color;

uniform sampler2D modelTexture;

void main(void) {
	out_Color = vec4(0.0, 0.0, 0.0, 1.0);
}