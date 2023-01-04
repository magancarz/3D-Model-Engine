#version 460 core

in vec2 texture_coords;

out vec4 out_Color;

uniform sampler2D color_texture;

void main(void) {
	vec4 color = texture(color_texture, texture_coords);
	float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
	if(brightness > 0.5) {
		out_Color = color;
	} else {
		out_Color = vec4(0.0);
	}
}