#pragma once

#include "shaders/ShaderProgram.h"

class CombineShader : public ShaderProgram {
public:
	CombineShader();

	void bindAttributes() override;

	void connectTextureUnits();

private:
	void getAllUniformLocations() override;

	unsigned int location_colorTexture,
				 location_highlightTexture;

};
