#version 460 core

in vec3 position;
in vec2 texture_coords;
in vec3 normal;

out vec2 pass_texture_coords;
out vec3 surface_normal;
out vec3 to_light_vector[4];
out vec3 to_camera_vector;
out float visibility;
out vec4 shadow_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 light_position[4];

uniform mat4 to_shadow_map_space;

uniform vec4 plane;

const float density = 0.007;
const float gradient = 1.5;

void main(void) {
	vec4 world_position = model * vec4(position, 1.0);
	shadow_coords = to_shadow_map_space * world_position;

	gl_ClipDistance[0] = dot(world_position, plane);

	vec4 position_relative_to_cam = view * world_position;
	gl_Position = proj * position_relative_to_cam;
	pass_texture_coords = texture_coords;

	surface_normal = (model * vec4(normal, 0.0)).xyz;
	for(int i = 0; i < 4; i++) {
		to_light_vector[i] = light_position[i] - world_position.xyz;
	}
	to_camera_vector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - world_position.xyz;

	float distance = length(position_relative_to_cam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}