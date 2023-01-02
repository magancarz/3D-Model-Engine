#include "ContrastShader.h"

ContrastShader::ContrastShader() :
ShaderProgram("res/shaders/contrastVert.glsl", "res/shaders/contrastFrag.glsl") {
	
}

void ContrastShader::get_all_uniform_locations() {
	
}

void ContrastShader::bind_attributes() {
	bind_attribute(0, "position");
}