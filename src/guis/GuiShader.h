#pragma once

#include <glm/glm.hpp>

#include "shaders/ShaderProgram.h"

class GuiShader : public ShaderProgram {
public:
	GuiShader();

	void loadTransformation(glm::mat4 matrix);

protected:
	void get_all_uniform_locations();
	void bind_attributes();

private:
	int location_transformationMatrix;
};
