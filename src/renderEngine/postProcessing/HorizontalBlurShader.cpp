#include "HorizontalBlurShader.h"

HorizontalBlurShader::HorizontalBlurShader() :
ShaderProgram("res/shaders/horizontalBlurVert.glsl", "res/shaders/blurFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void HorizontalBlurShader::load_target_width(const float width) const {
	load_float(location_target_width, width);
}

void HorizontalBlurShader::bind_attributes() {
	bind_attribute(0, "position");
	bind_attribute(1, "textureCoords");
}

void HorizontalBlurShader::get_all_uniform_locations() {
	location_target_width = get_uniform_location("targetWidth");
}