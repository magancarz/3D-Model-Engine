#pragma once

#include "../Headers.h"
#include "../models/RawModel.h"
#include "SkyboxShader.h"

class SkyboxRenderer {
public:
	SkyboxRenderer(Loader* loader, glm::mat4 projectionMatrix);
	~SkyboxRenderer();

	void render(Camera& camera);
private:
	static constexpr float SIZE = 500.0f;

	std::vector<std::string> TEXTURE_FILES = {
		"right", "left", "top",
		"bottom", "back", "front"
	};
	RawModel* m_cube;
	int m_texture;
	SkyboxShader* m_shader;
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