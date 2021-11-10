#version 460 core

in vec2 pass_textureCoords;

out vec4 out_Color;

uniform vec3 color;
uniform sampler2D fontAtlas;

void main(void) {
	out_Color = vec4(color, texture(fontAtlas, pass_textureCoords).a);
}