#version 460 core

in vec2 position;

out vec4 clip_space;
out vec2 texture_coords;
out vec3 to_camera_vector;
out vec3 from_light_vector;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 camera_position;
uniform vec3 light_position;

const float tiling = 4.0;

void main(void) {
	//local space -> world space
	vec4 world_position = model * vec4(position.x, 0.0, position.y, 1.0);

	//world space -> eye space
	vec4 position_relative_to_cam = view * world_position;

	clip_space = proj * view * world_position;
	gl_Position = clip_space;
	texture_coords = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;
	to_camera_vector = camera_position - world_position.xyz;
	from_light_vector = world_position.xyz - light_position;
}