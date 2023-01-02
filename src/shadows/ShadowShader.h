#pragma once

#include "../shaders/ShaderProgram.h"

class ShadowShader : public ShaderProgram {
public:
	ShadowShader();

	void connectTextureUnits();
	void bind_attributes();
	void get_all_uniform_locations();
	
	void loadMVPMatrix(glm::mat4 mvpMatrix);

private:
	unsigned int location_mvpMatrix,
				 location_modelTexture;

};