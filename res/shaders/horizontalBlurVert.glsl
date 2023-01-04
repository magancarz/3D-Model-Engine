#version 460 core

in vec2 position;

out vec2 blur_texture_coords[11];

uniform float target_width;

void main(void) {
	gl_Position = vec4(position, 0.0, 1.0);
	vec2 center_tex_coords = position * 0.5 + 0.5;
	float pixel_size = 1.0 / target_width;

	for(int i = (-5); i <= 5; i++) {
		blur_texture_coords[i + 5] = center_tex_coords + vec2(pixel_size * i, 0.0);
	}
}