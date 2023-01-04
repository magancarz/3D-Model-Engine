#version 460 core

in vec2 texture_coords;

out vec4 out_Color;

uniform sampler2D color_texture;
uniform sampler2D highlight_texture;

void main(void) {
	vec4 scene_color = texture(color_texture, texture_coords);
	vec4 highlight_color = texture(highlight_texture, texture_coords);

	out_Color = scene_color + highlight_color * 3.5;
}