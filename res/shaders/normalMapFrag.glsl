#version 460 core

in vec2 pass_texture_coordinates;
in vec3 surface_normal;
in vec3 to_light_vector[4];
in vec3 to_camera_vector;
in float visibility;

layout (location = 0) out vec4 out_Color;
layout (location = 1) out vec4 out_BrightColor;

uniform sampler2D model_texture;
uniform sampler2D normal_map;
uniform vec3 light_color[4];
uniform vec3 attenuation[4];
uniform float shine_damper;
uniform float reflectivity;
uniform vec3 sky_color;

void main() {
	vec4 normal_map_value = 2.0 * texture(normal_map, pass_texture_coordinates) - 1.0;

	vec3 unit_normal = normalize(normal_map_value.rgb);
	vec3 unit_vector_to_camera = normalize(to_camera_vector);

	vec3 total_diffuse = vec3(0.0);
	vec3 total_specular = vec3(0.0);

	for (int i = 0; i < 4; i++) {
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

	total_diffuse = max(total_diffuse, 0.1);

	vec4 texture_color = texture(model_texture, pass_texture_coordinates);
	if (texture_color.a < 0.5) {
		discard;
	}
	
	out_Color = vec4(total_diffuse,1.0) * texture_color + vec4(total_specular, 1.0);
	out_Color = mix(vec4(sky_color, 1.0), out_Color, visibility);

	out_BrightColor = vec4(0.0);
}