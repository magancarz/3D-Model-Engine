#include "ShadowShader.h"

ShadowShader::ShadowShader() :
ShaderProgram("res/shaders/shadowVert.glsl", "res/shaders/shadowFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void ShadowShader::bind_attributes() {
	bind_attribute(0, "in_position");
	bind_attribute(1, "in_textureCoords");
}

void ShadowShader::get_all_uniform_locations() {
	location_mvp_matrix = get_uniform_location("mvp");
}
	
void ShadowShader::load_mvp_matrix(const glm::mat4& mvp_matrix) const {
	load_matrix(location_mvp_matrix, mvp_matrix);
}