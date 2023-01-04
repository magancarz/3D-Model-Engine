#pragma once

#include <memory>

#include "TerrainTexture.h"

class TerrainTexturePack {
public:
	TerrainTexturePack(
		std::unique_ptr<TerrainTexture> background_texture,
		std::unique_ptr<TerrainTexture> r_texture,
		std::unique_ptr<TerrainTexture> g_texture,
		std::unique_ptr<TerrainTexture> b_texture);

	std::shared_ptr<TerrainTexture> get_background_texture();
	std::shared_ptr<TerrainTexture> get_r_texture();
	std::shared_ptr<TerrainTexture> get_g_texture();
	std::shared_ptr<TerrainTexture> get_b_texture();

private:
	std::shared_ptr<TerrainTexture> m_background_texture,
								    m_r_texture,
								    m_g_texture,
								    m_b_texture;
};