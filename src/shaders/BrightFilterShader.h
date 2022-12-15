#pragma once

#include "Headers.h"
#include "ShaderProgram.h"

class BrightFilterShader : public ShaderProgram {
public:
	BrightFilterShader();

	void bindAttributes() override;

private:
	void getAllUniformLocations() override;
};