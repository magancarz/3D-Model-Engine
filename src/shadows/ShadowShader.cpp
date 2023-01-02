#include "ShadowShader.h"

ShadowShader::ShadowShader()
	: ShaderProgram("res/shaders/shadowVert.glsl", "res/shaders/shadowFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void ShadowShader::bind_attributes() {
	bind_attribute(0, "in_position");
	bind_attribute(1, "in_textureCoords");
}

void ShadowShader::get_all_uniform_locations() {
	location_mvpMatrix = get_uniform_location("mvp");
}
	
void ShadowShader::loadMVPMatrix(glm::mat4 mvpMatrix) {
	load_matrix(location_mvpMatrix, mvpMatrix);
}