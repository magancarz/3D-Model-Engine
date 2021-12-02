#pragma once

#include "../Headers.h"

#include "../renderEngine/Loader.h"
#include "../renderEngine/TextureData.h"
#include "../models/RawModel.h"
#include "../textures/ModelTexture.h"
#include "../textures/TerrainTexturePack.h"
#include "../textures/TerrainTexture.h"
#include "HeightsGenerator.h"

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

	float getHeight(int x, int z, HeightsGenerator* generator);
	glm::vec3 calculateNormal(int x, int z, HeightsGenerator* generator);

	const float SIZE = 800,
				MAX_HEIGHT = 40,
				MAX_PIXEL_COLOR = 256 + 256 + 256;

	float** m_heights;
	int m_heightsSize;

	float m_x, m_z;
	RawModel* m_model;
	TerrainTexturePack* m_texturePack;
	TerrainTexture* m_blendMap;
};