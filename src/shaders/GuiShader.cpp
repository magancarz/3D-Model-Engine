#include "GuiShader.h"

GuiShader::GuiShader()
	: ShaderProgram("res/shaders/guiVert.shader", "res/shaders/guiFrag.shader") {
	getAllUniformLocations();
	bindAttributes();
}

void GuiShader::loadTransformation(glm::mat4 matrix) {
	loadMatrix(location_transformationMatrix, matrix);
}

void GuiShader::getAllUniformLocations() {
	location_transformationMatrix = getUniformLocation("model");
}

void GuiShader::bindAttributes() {
	bindAttribute(0, "position");
}