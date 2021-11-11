#include "FontShader.h"

const std::string VERTEX_FILE = "res/shaders/fontVert.shader";
const std::string FRAGMENT_FILE = "res/shaders/fontFrag.shader";

FontShader::FontShader() : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
	bindAttributes();
	getAllUniformLocations();
}

void FontShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
}

void FontShader::getAllUniformLocations()
{
	location_color = getUniformLocation("color");
	location_translation = getUniformLocation("translation");
}

void FontShader::loadColor(glm::vec3 color)
{
	loadVector3f(location_color, color);
}

void FontShader::loadTranslation(glm::vec2 translation)
{
	loadVector2f(location_translation, translation);
}
