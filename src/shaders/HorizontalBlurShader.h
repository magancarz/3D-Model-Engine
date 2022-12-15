#pragma once

#include "ShaderProgram.h"

class HorizontalBlurShader : public ShaderProgram {
public:
	HorizontalBlurShader();

	void loadTargetWidth(float width);

private:
	void getAllUniformLocations() override;

	void bindAttributes() override;

	unsigned int location_targetWidth;
};