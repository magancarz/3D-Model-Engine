#include "CombineShader.h"

CombineShader::CombineShader() :
ShaderProgram("res/shaders/simpleVert.glsl", "res/shaders/combineFrag.glsl") {
	get_all_uniform_locations();
}

void CombineShader::bind_attributes() {
	bind_attribute(0, "position");
}

void CombineShader::connectTextureUnits() {
	load_int(location_colorTexture, 0);
	load_int(location_highlightTexture, 1);
}

void CombineShader::get_all_uniform_locations() {
	location_colorTexture = get_uniform_location("colorTexture");
	location_highlightTexture = get_uniform_location("highlightTexture");
}