#pragma once

#include "ShaderProgram.h"

class VerticalBlurShader : public ShaderProgram {
public:
	VerticalBlurShader();

	void loadTargetHeight(float width);

private:
	void getAllUniformLocations() override;

	void bindAttributes() override;

	unsigned int location_targetHeight;
};