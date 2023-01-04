#version 460 core

in vec3 texture_coords;
out vec4 out_Color;

uniform samplerCube cube_map;
uniform samplerCube cube_map2;
uniform float blend_factor;
uniform vec3 fog_color;

const float lower_limit = 0.0;
const float upper_limit = 30.0;

void main(void){
    vec4 texture1 = texture(cube_map, texture_coords);
    vec4 texture2 = texture(cube_map2, texture_coords);
    vec4 final_color = mix(texture1, texture2, blend_factor);

    float factor = (texture_coords.y - lower_limit) / (upper_limit - lower_limit);
    factor = clamp(factor, 0.0, 1.0);
    out_Color = mix(vec4(fog_color, 1.0), final_color, factor);
}