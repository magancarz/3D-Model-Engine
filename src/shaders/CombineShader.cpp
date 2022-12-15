#include "CombineShader.h"

CombineShader::CombineShader() :
ShaderProgram("res/shaders/simpleVert.glsl", "res/shaders/combineFrag.glsl") {
	getAllUniformLocations();
}

void CombineShader::bindAttributes() {
	bindAttribute(0, "position");
}

void CombineShader::connectTextureUnits() {
	loadInt(location_colorTexture, 0);
	loadInt(location_highlightTexture, 1);
}

void CombineShader::getAllUniformLocations() {
	location_colorTexture = getUniformLocation("colorTexture");
	location_highlightTexture = getUniformLocation("highlightTexture");
}