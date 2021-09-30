#pragma once

#include "../Headers.h"

#include "../renderEngine/Loader.h"
#include "../models/RawModel.h"
#include "../textures/ModelTexture.h"

class Terrain {
public:
	Terrain(int gridX, int gridZ, Loader* loader, ModelTexture* texture);

	inline float getX() const { return m_x; };
	inline float getZ() const { return m_z; };
	inline RawModel* getModel() { return m_model; };
	inline ModelTexture* getTexture() { return m_texture; };

private:
	RawModel* generateTerrain(Loader* loader);

	const float SIZE = 800;
	const int VERTEX_COUNT = 128;

	float m_x, m_z;
	RawModel* m_model;
	ModelTexture* m_texture;
};