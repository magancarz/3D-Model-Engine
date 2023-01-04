#include "CombineShader.h"

CombineShader::CombineShader() :
ShaderProgram("res/shaders/simpleVert.glsl", "res/shaders/combineFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void CombineShader::bind_attributes() {
	bind_attribute(0, "position");
}

void CombineShader::connect_texture_units() const {
	load_int(location_color_texture, 0);
	load_int(location_highlight_texture, 1);
}

void CombineShader::get_all_uniform_locations() {
	location_color_texture = get_uniform_location("color_texture");
	location_highlight_texture = get_uniform_location("highlight_texture");
}