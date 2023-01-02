#pragma once

#include "shaders/ShaderProgram.h"

class BrightFilterShader : public ShaderProgram {
public:
	BrightFilterShader();

	void bindAttributes() override;

private:
	void getAllUniformLocations() override;
};
