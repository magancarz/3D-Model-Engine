#include "ContrastShader.h"

ContrastShader::ContrastShader() :
ShaderProgram("res/shaders/contrastVert.glsl", "res/shaders/contrastFrag.glsl") {
	
}

void ContrastShader::getAllUniformLocations() {
	
}

void ContrastShader::bindAttributes() {
	bindAttribute(0, "position");
}