#pragma once

#include "../shaders/ShaderProgram.h"

class BrightFilterShader : public ShaderProgram {
public:
	BrightFilterShader();

	void bind_attributes() override;
	void get_all_uniform_locations() override;
};