#pragma once

#include "ShaderProgram.h"

class GuiShader : public ShaderProgram {
public:
	GuiShader();

	void loadTransformation(glm::mat4 matrix);

protected:
	void getAllUniformLocations();
	void bindAttributes();

private:
	int location_transformationMatrix;
};