#version 460 core

in vec3 position;
in vec2 texture_coordinates;
in vec3 normal;
in vec3 tangent;

out vec2 pass_texture_coordinates;
out vec3 to_light_vector[4];
out vec3 to_camera_vector;
out float visibility;

uniform mat4 model;          // objects translation, rotation and scaling in the world cooridinates
uniform mat4 proj;           // frustum
uniform mat4 view;           // camera
uniform vec3 light_position_eye_space[4];

uniform float number_of_rows;
uniform vec2 texture_offset;

uniform vec4 clip_plane;

const float density = 0.002;
const float gradient = 1.5;

void main() {
	vec4 world_position = model * vec4(position, 1.0);
	gl_ClipDistance[0] = dot(world_position, clip_plane);
	mat4 modelview = view * model;
	vec4 position_relative_to_cam = modelview * vec4(position, 1.0);
	gl_Position = proj * position_relative_to_cam;
	
	pass_texture_coordinates = (texture_coordinates / number_of_rows) + texture_offset;

	vec3 surface_normal = (modelview * vec4(normal, 0.0)).xyz;

	vec3 norm = normalize(surface_normal);
	vec3 tang = normalize((modelview * vec4(tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));

	mat3 to_tangent_space = mat3(
		tang.x, bitang.x, norm.x,
		tang.y, bitang.y, norm.y,
		tang.z, bitang.z, norm.z
	);

	for (int i = 0; i < 4; i++) {
		to_light_vector[i] = to_tangent_space * (light_position_eye_space[i] - position_relative_to_cam.xyz);
	}
	to_camera_vector = to_tangent_space * (-position_relative_to_cam.xyz);

	float distance = length(position_relative_to_cam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}