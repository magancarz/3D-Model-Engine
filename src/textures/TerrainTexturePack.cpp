#include "TerrainTexturePack.h"

TerrainTexturePack::TerrainTexturePack(
	std::unique_ptr<TerrainTexture> background_texture,
	std::unique_ptr<TerrainTexture> r_texture,
	std::unique_ptr<TerrainTexture> g_texture,
	std::unique_ptr<TerrainTexture> b_texture) :
m_background_texture(std::move(background_texture)),
m_r_texture(std::move(r_texture)),
m_g_texture(std::move(g_texture)),
m_b_texture(std::move(b_texture)) {}

std::shared_ptr<TerrainTexture> TerrainTexturePack::get_background_texture() {
	return m_background_texture;
}

std::shared_ptr<TerrainTexture> TerrainTexturePack::get_r_texture() {
	return m_r_texture;
}

std::shared_ptr<TerrainTexture> TerrainTexturePack::get_g_texture() {
	return m_g_texture;
}

std::shared_ptr<TerrainTexture> TerrainTexturePack::get_b_texture() {
	return m_b_texture;
}