#include "HorizontalBlurShader.h"

HorizontalBlurShader::HorizontalBlurShader() :
ShaderProgram("res/shaders/horizontalBlurVert.glsl", "res/shaders/blurFrag.glsl") {
	get_all_uniform_locations();
}

void HorizontalBlurShader::loadTargetWidth(float width) {
	load_float(location_targetWidth, width);
}

void HorizontalBlurShader::get_all_uniform_locations() {
	location_targetWidth = get_uniform_location("targetWidth");
}

void HorizontalBlurShader::bind_attributes() {
	bind_attribute(0, "position");
	bind_attribute(1, "textureCoords");
}