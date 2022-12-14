#include "ShadowShader.h"

ShadowShader::ShadowShader()
	: ShaderProgram("res/shaders/shadowVert.glsl", "res/shaders/shadowFrag.glsl") {
	bindAttributes();
	getAllUniformLocations();
}

void ShadowShader::bindAttributes() {
	bindAttribute(0, "in_position");
	bindAttribute(1, "in_textureCoords");
}

void ShadowShader::getAllUniformLocations() {
	location_mvpMatrix = getUniformLocation("mvp");
}
	
void ShadowShader::loadMVPMatrix(glm::mat4 mvpMatrix) {
	loadMatrix(location_mvpMatrix, mvpMatrix);
}