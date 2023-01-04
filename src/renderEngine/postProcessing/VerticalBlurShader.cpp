#include "VerticalBlurShader.h"

VerticalBlurShader::VerticalBlurShader() :
ShaderProgram("res/shaders/verticalBlurVert.glsl", "res/shaders/blurFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void VerticalBlurShader::load_target_height(const float width) const {
	load_float(location_target_height, width);
}

void VerticalBlurShader::bind_attributes() {
	bind_attribute(0, "position");
}

void VerticalBlurShader::get_all_uniform_locations() {
	location_target_height = get_uniform_location("target_height");
}