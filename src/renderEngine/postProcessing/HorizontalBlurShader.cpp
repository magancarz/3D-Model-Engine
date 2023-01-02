#include "HorizontalBlurShader.h"

HorizontalBlurShader::HorizontalBlurShader() :
ShaderProgram("res/shaders/horizontalBlurVert.glsl", "res/shaders/blurFrag.glsl") {
	getAllUniformLocations();
}

void HorizontalBlurShader::loadTargetWidth(float width) {
	loadFloat(location_targetWidth, width);
}

void HorizontalBlurShader::getAllUniformLocations() {
	location_targetWidth = getUniformLocation("targetWidth");
}

void HorizontalBlurShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
}