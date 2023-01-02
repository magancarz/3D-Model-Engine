#pragma once

#include "shaders/ShaderProgram.h"

class HorizontalBlurShader : public ShaderProgram {
public:
	HorizontalBlurShader();

	void loadTargetWidth(float width);

private:
	void get_all_uniform_locations() override;

	void bind_attributes() override;

	unsigned int location_targetWidth;
};
