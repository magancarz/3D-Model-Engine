#version 460 core

in vec2 texture_coords;

out vec4 out_Color;

uniform sampler2D color_texture;

const float contrast = 0.001;

void main(void) {
	out_Color = texture(color_texture, texture_coords);
	out_Color.rgb = (out_Color.rgb - 0.5) * (1.0 + contrast) + 0.5;
}