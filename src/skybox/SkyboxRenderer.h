#pragma once

#include "../models/RawModel.h"
#include "SkyboxShader.h"

class SkyboxRenderer {
public:
	SkyboxRenderer(const std::shared_ptr<Loader>& loader, const glm::mat4& projection_matrix);

	void render(const std::shared_ptr<Camera>& camera, float r, float g, float b) const;

private:
	void bind_textures() const;

	static constexpr float SIZE = 500.0f;

	float m_time = 0;

	std::shared_ptr<RawModel> m_cube;
	std::unique_ptr<SkyboxShader> m_skybox_shader;

	int m_texture, m_night_texture;

	std::vector<std::string> TEXTURE_FILES = {
		"right", "left", "top",
		"bottom", "back", "front"
	};

	std::vector<std::string> NIGHT_TEXTURE_FILES = {
		"nightRight", "nightLeft", "nightTop",
		"nightBottom", "nightBack", "nightFront"
	};

	inline static const std::vector<float> VERTICES = {
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