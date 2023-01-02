#pragma once

#include "shaders/ShaderProgram.h"

class CombineShader : public ShaderProgram {
public:
	CombineShader();

	void bind_attributes() override;

	void connectTextureUnits();

private:
	void get_all_uniform_locations() override;

	unsigned int location_colorTexture,
				 location_highlightTexture;

};
