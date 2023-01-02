#pragma once

#include "terrain/TerrainShader.h"

class TerrainRenderer {
public:
	TerrainRenderer(TerrainShader* shader, glm::mat4 projectionMatrix);

	void render(const std::vector<std::shared_ptr<Terrain>>& terrains, const glm::mat4& toShadowMapSpace);

	inline void setShader(TerrainShader* shader) { m_shader = shader; };
private:
	void prepareTerrain(Terrain* terrain);
	void bindTextures(Terrain* terrain);
	void unbindTexturedModel();
	void loadModelMatrix(Terrain* terrain);

	TerrainShader* m_shader;
};
