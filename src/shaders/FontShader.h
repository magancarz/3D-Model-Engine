#pragma once

#include "ShaderProgram.h"

class FontShader : public ShaderProgram {
public:
	FontShader();

	void bindAttributes();
	void getAllUniformLocations();

	void loadColor(glm::vec3 color);
	void loadTranslation(glm::vec2 translation);

private:
	unsigned int location_color;
	unsigned int location_translation;

};