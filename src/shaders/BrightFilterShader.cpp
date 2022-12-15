#include "BrightFilterShader.h"

BrightFilterShader::BrightFilterShader() :
ShaderProgram("res/shaders/simpleVert.glsl", "res/shaders/brightFilterFrag.glsl") {
	
}

void BrightFilterShader::getAllUniformLocations() {

}

void BrightFilterShader::bindAttributes() {
	bindAttribute(0, "position");
}