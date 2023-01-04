#pragma once

#include "../shaders/ShaderProgram.h"

class ContrastShader : public ShaderProgram {
public:
	ContrastShader();

	void bind_attributes() override;
	void get_all_uniform_locations() override;
};