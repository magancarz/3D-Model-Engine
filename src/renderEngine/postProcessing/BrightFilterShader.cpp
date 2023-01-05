#include "BrightFilterShader.h"

BrightFilterShader::BrightFilterShader() :
ShaderProgram("res/shaders/simpleVert.glsl", "res/shaders/brightFilterFrag.glsl") {}

void BrightFilterShader::bind_attributes() {
	bind_attribute(0, "position");
}

void BrightFilterShader::get_all_uniform_locations() {

}