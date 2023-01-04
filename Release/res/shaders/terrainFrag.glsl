#version 460 core

in vec2 pass_texture_coords;
in vec3 surface_normal;
in vec3 to_light_vector[4];
in vec3 to_camera_vector;
in float visibility;
in vec4 shadow_coords;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_Bright_color;

uniform sampler2D background_texture;
uniform sampler2D r_texture;
uniform sampler2D g_texture;
uniform sampler2D b_texture;
uniform sampler2D blend_map;
uniform sampler2D shadow_map;

uniform vec3 light_color[4];
uniform vec3 attenuation[4];
uniform float shine_damper;
uniform float reflectivity;
uniform vec3 sky_color;

const int pcf_count = 4;
const float total_texels = (pcf_count * 2.0 + 1.0) * (pcf_count * 2.0 + 1.0);

void main(void) {
	float map_size = 4096.0;
	float texelSize = 1.0 / map_size;
	float total = 0.0;

	for(int x = -pcf_count; x <= pcf_count; x++) {
		for(int y = -pcf_count; y <= pcf_count; y++) {
			float closest_depth = texture(shadow_map, shadow_coords.xy + vec2(x, y) * texelSize).r;
			float current_depth = shadow_coords.z;
			if(current_depth > closest_depth) {
				total += 1.0;
			}
		}
	}

	total /= total_texels;

	float light_factor = 1.0 - (total * shadow_coords.w);

	vec4 blend_map_color = texture(blend_map, pass_texture_coords);

	float back_texture_amount = 1 - (blend_map_color.r + blend_map_color.g + blend_map_color.b);
	vec2 tiled_coords = pass_texture_coords * 40.0;
	vec4 background_texture_color = texture(background_texture, tiled_coords) * back_texture_amount;
	vec4 r_texture_color = texture(r_texture, tiled_coords) * blend_map_color.r;
	vec4 g_texture_color = texture(g_texture, tiled_coords) * blend_map_color.g;
	vec4 b_texture_color = texture(b_texture, tiled_coords) * blend_map_color.b;

	vec4 total_color = background_texture_color + r_texture_color + g_texture_color + b_texture_color;

	vec3 unit_normal = normalize(surface_normal);
	vec3 unit_vector_to_camera = normalize(to_camera_vector);

	vec3 total_diffuse = vec3(0.0);
	vec3 total_specular = vec3(0.0);

	for(int i = 0; i < 4; i++) {
		float distance = length(to_light_vector[i]);
		float att_factor = attenuation[i].x + (attenuation[i].y * distance) + (attenuation[i].z * distance * distance);
		vec3 unit_light_vector = normalize(to_light_vector[i]);
		float n_dot1 = dot(unit_normal, unit_light_vector);
		float brightness = max(n_dot1, 0.0);
		vec3 light_direction = -unit_light_vector;
		vec3 reflected_light_direction = reflect(light_direction, unit_normal);
		float specular_factor = dot(reflected_light_direction, unit_vector_to_camera);
		specular_factor = max(specular_factor, 0.0);
		float damped_factor = pow(specular_factor, shine_damper);
		total_diffuse = total_diffuse + (brightness * light_color[i]) / att_factor;
		total_specular = total_specular + (damped_factor * reflectivity * light_color[i]) / att_factor;
	}

	total_diffuse = max(total_diffuse * light_factor, 0.1);

	out_color = vec4(total_diffuse, 1.0) * total_color + vec4(total_specular, 1.0);
	out_color = mix(vec4(sky_color, 1.0), out_color, visibility);

	out_Bright_color = vec4(0.0);
}