#pragma once

#include <memory>

#include "terrain/TerrainShader.h"

class TerrainRenderer {
public:
	TerrainRenderer(const glm::mat4& projection_matrix);

	void render(
		const std::vector<std::shared_ptr<Terrain>>& terrains,
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const;

private:
	void prepare_terrain(const std::shared_ptr<Terrain>& terrain) const;
	void load_model_matrix(const std::shared_ptr<Terrain>& terrain) const;

	static void bind_textures(const std::shared_ptr<Terrain>& terrain);
	static void unbind_textured_model();

	std::unique_ptr<TerrainShader> m_terrain_shader;
};
