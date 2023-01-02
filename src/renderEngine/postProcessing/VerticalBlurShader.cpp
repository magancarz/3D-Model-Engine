#include "VerticalBlurShader.h"

VerticalBlurShader::VerticalBlurShader() :
ShaderProgram("res/shaders/verticalBlurVert.glsl", "res/shaders/blurFrag.glsl") {
	get_all_uniform_locations();
}

void VerticalBlurShader::loadTargetHeight(float width) {
	load_float(location_targetHeight, width);
}

void VerticalBlurShader::get_all_uniform_locations() {
	location_targetHeight = get_uniform_location("targetHeight");
}

void VerticalBlurShader::bind_attributes() {
	bind_attribute(0, "position");
}