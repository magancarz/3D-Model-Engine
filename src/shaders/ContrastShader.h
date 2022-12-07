#pragma once

#include "ShaderProgram.h"

class ContrastShader : public ShaderProgram {
public:
	ContrastShader();

private:
	void getAllUniformLocations() override;

	void bindAttributes() override;
};