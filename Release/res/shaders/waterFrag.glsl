#version 460 core

in vec4 clip_space;
in vec2 texture_coords;
in vec3 to_camera_vector;
in vec3 from_light_vector;

layout (location = 0) out vec4 out_Color;
layout (location = 1) out vec4 out_BrightColor;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D dudv_map;
uniform sampler2D normal_map;
uniform sampler2D depth_map;
uniform float move_factor;
uniform vec3 light_color;

const float wave_strength = 0.04;
const float shine_damper = 70.0;
const float reflectivity = 0.02;

void main(void) {
	vec2 ndc = (clip_space.xy / clip_space.w) / 2.0 + 0.5;
	vec2 refraction_tex_coords = vec2(ndc.x, ndc.y);
	vec2 reflection_tex_coords = vec2(ndc.x, -ndc.y);

	float near = 0.1;
	float far = 1000.0;

	float depth = texture(depth_map, refraction_tex_coords).r;
	float floor_distance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.z;
	float water_distance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float water_depth = floor_distance - water_distance;

	vec2 distorted_tex_coords = texture(dudv_map, vec2(texture_coords.x + move_factor, texture_coords.y)).rg * 0.1;
	distorted_tex_coords = texture_coords + vec2(distorted_tex_coords.x, distorted_tex_coords.y + move_factor);
	vec2 total_distortion = (texture(dudv_map, distorted_tex_coords).rg * 2.0 - 1.0) * wave_strength * clamp(water_depth / 1.0, 0.0, 1.0);
	
	refraction_tex_coords += total_distortion;
	refraction_tex_coords = clamp(refraction_tex_coords, 0.001, 0.999);
	reflection_tex_coords += total_distortion;
	reflection_tex_coords.x = clamp(reflection_tex_coords.x, 0.001, 0.999);
	reflection_tex_coords.y = clamp(reflection_tex_coords.y, -0.999, -0.001);

	vec4 reflect_color = texture(reflection_texture, reflection_tex_coords);
	vec4 refract_color = texture(refraction_texture, refraction_tex_coords);

	vec4 normal_map_color = texture(normal_map, distorted_tex_coords);
	vec3 normal = vec3(normal_map_color.r * 2.0 - 1.0, normal_map_color.b * 3.0, normal_map_color.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 view_vector = normalize(to_camera_vector);
	float refractive_factor = dot(view_vector, normal);
	refractive_factor = pow(refractive_factor, 0.5);
	refractive_factor = clamp(refractive_factor, 0.0, 1.0);

	vec3 reflected_light = reflect(normalize(from_light_vector), normal);
	float specular = max(dot(reflected_light, view_vector), 0.0);
	specular = pow(specular, shine_damper);
	vec3 specular_highlights = light_color * specular * reflectivity * clamp(water_depth / 5.0, 0.0, 1.0);
	
	out_Color = mix(reflect_color, refract_color, 0.5);
	out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specular_highlights, 0.0);
	out_Color.a = clamp(water_depth / 5.0, 0.0, 1.0);

	out_BrightColor = vec4(0.0);
}