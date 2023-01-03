#pragma once

#include <glm/glm.hpp>

#include "../renderEngine/Loader.h"
#include "../renderEngine/TextureData.h"
#include "../models/RawModel.h"
#include "../textures/TerrainTexturePack.h"
#include "../textures/TerrainTexture.h"

#define TERRAIN_SIZE 800
#define TERRAIN_MAX_HEIGHT 40

class Terrain {
public:
	Terrain(
		float grid_x,
		float grid_z,
		const std::shared_ptr<Loader>& loader,
		std::shared_ptr<TerrainTexturePack> texture_pack,
		std::shared_ptr<TerrainTexture> blend_map);

	~Terrain();

	float get_x() const;
	float get_z() const;
	std::shared_ptr<RawModel> get_model() const;
	std::shared_ptr<TerrainTexturePack> get_terrain_texture_pack() const;
	std::shared_ptr<TerrainTexture> get_blend_map() const;

	float get_height_of_terrain(float world_x, float world_z) const;

private:
	std::shared_ptr<RawModel> generate_terrain(
		const std::shared_ptr<Loader>& loader,
		const std::string& height_map_location);

	float get_height(int x, int z, const std::unique_ptr<TextureData>& texture_data) const;
	glm::vec3 calculate_normal(int x, int z, const std::unique_ptr<TextureData>& texture_data) const;

	const float MAX_PIXEL_COLOR = 256 + 256 + 256;

	float** m_heights;
	int m_heights_size;

	float m_x, m_z;
	std::shared_ptr<RawModel> m_model;
	std::shared_ptr<TerrainTexturePack> m_texture_pack;
	std::shared_ptr<TerrainTexture> m_blend_map;
};