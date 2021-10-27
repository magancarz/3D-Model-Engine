#pragma once

#include "../Headers.h"
#include "../models/RawModel.h"
#include "SkyboxShader.h"

class SkyboxRenderer {
public:
	SkyboxRenderer(Loader* loader, glm::mat4 projectionMatrix);
	~SkyboxRenderer();

	void render(Camera& camera, float r, float g, float b);
private:
	void bindTextures();

	static constexpr float SIZE = 500.0f;
	float time = 0;

	RawModel* m_cube;
	int m_texture, m_nightTexture;
	SkyboxShader* m_shader;

	std::vector<std::string> TEXTURE_FILES = {
		"right", "left", "top",
		"bottom", "back", "front"
	};
	std::vector<std::string> NIGHT_TEXTURE_FILES = {
		"nightRight", "nightLeft", "nightTop",
		"nightBottom", "nightBack", "nightFront"
	};
	std::vector<float> VERTICES = {
		-SIZE, SIZE,-SIZE,
		-SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,
		 SIZE, SIZE,-SIZE,
		-SIZE, SIZE,-SIZE,

		-SIZE,-SIZE, SIZE,
		-SIZE,-SIZE,-SIZE,
		-SIZE, SIZE,-SIZE,
		-SIZE, SIZE,-SIZE,
		-SIZE, SIZE, SIZE,
		-SIZE,-SIZE, SIZE,

		 SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,

		-SIZE,-SIZE, SIZE,
		-SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE,-SIZE, SIZE,
		-SIZE,-SIZE, SIZE,

		-SIZE, SIZE,-SIZE,
		 SIZE, SIZE,-SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		-SIZE, SIZE, SIZE,
		-SIZE, SIZE,-SIZE,

		-SIZE,-SIZE,-SIZE,
		-SIZE,-SIZE, SIZE,
		 SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,
		-SIZE,-SIZE, SIZE,
		 SIZE,-SIZE, SIZE
	};
};