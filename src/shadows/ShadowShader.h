#pragma once

#include "../shaders/ShaderProgram.h"

class ShadowShader : public ShaderProgram {
public:
	ShadowShader();

	void connectTextureUnits();
	void bindAttributes();
	void getAllUniformLocations();
	
	void loadMVPMatrix(glm::mat4 mvpMatrix);

private:
	unsigned int location_mvpMatrix,
				 location_modelTexture;

};