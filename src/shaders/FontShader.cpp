#include "FontShader.h"

FontShader::FontShader()
	: ShaderProgram("res/shaders/fontVert.glsl", "res/shaders/fontFrag.glsl") {
	bindAttributes();
	getAllUniformLocations();
}

void FontShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
}

void FontShader::getAllUniformLocations() {
	location_color = getUniformLocation("color");
	location_translation = getUniformLocation("translation");
}

void FontShader::loadColor(glm::vec3 color) {
	loadVector3f(location_color, color);
}

void FontShader::loadTranslation(glm::vec2 translation) {
	loadVector2f(location_translation, translation);
}