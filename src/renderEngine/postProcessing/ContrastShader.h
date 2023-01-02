#pragma once

#include "shaders/ShaderProgram.h"

class ContrastShader : public ShaderProgram {
public:
	ContrastShader();

private:
	void get_all_uniform_locations() override;

	void bind_attributes() override;
};
