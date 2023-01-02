#include "GuiShader.h"

GuiShader::GuiShader()
	: ShaderProgram("res/shaders/guiVert.glsl", "res/shaders/guiFrag.glsl") {
	get_all_uniform_locations();
	bind_attributes();
}

void GuiShader::loadTransformation(glm::mat4 matrix) {
	load_matrix(location_transformationMatrix, matrix);
}

void GuiShader::get_all_uniform_locations() {
	location_transformationMatrix = get_uniform_location("model");
}

void GuiShader::bind_attributes() {
	bind_attribute(0, "position");
}