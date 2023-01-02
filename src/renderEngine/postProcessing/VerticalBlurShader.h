#pragma once

#include "shaders/ShaderProgram.h"

class VerticalBlurShader : public ShaderProgram {
public:
	VerticalBlurShader();

	void loadTargetHeight(float width);

private:
	void get_all_uniform_locations() override;

	void bind_attributes() override;

	unsigned int location_targetHeight;
};
