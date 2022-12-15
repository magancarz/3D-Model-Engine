#include "Headers.h"
#include "VerticalBlurShader.h"

VerticalBlurShader::VerticalBlurShader() :
ShaderProgram("res/shaders/verticalBlurVert.glsl", "res/shaders/blurFrag.glsl") {
	getAllUniformLocations();
}

void VerticalBlurShader::loadTargetHeight(float width) {
	loadFloat(location_targetHeight, width);
}

void VerticalBlurShader::getAllUniformLocations() {
	location_targetHeight = getUniformLocation("targetHeight");
}

void VerticalBlurShader::bindAttributes() {
	bindAttribute(0, "position");
}