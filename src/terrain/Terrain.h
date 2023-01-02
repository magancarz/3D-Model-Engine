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
	Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap);
	~Terrain();

	inline float getX() const { return m_x; };
	inline float getZ() const { return m_z; };
	inline RawModel* getModel() { return m_model; };
	inline TerrainTexturePack* getTerrainTexturePack() { return m_texturePack; };
	inline TerrainTexture* getBlendMap() { return m_blendMap; };

	float getHeightOfTerrain(float worldX, float worldZ);

private:
	RawModel* generateTerrain(Loader* loader, std::string);

	float getHeight(int x, int z, TextureData* textureData);
	glm::vec3 calculateNormal(int x, int z, TextureData* textureData);

	const float MAX_PIXEL_COLOR = 256 + 256 + 256;

	float** m_heights;
	int m_heightsSize;

	float m_x, m_z;
	RawModel* m_model;
	TerrainTexturePack* m_texturePack;
	TerrainTexture* m_blendMap;
};