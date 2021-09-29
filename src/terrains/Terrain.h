#pragma once

#include "../Headers.h"

#include "../renderEngine/Loader.h"
#include "../models/RawModel.h"
#include "../textures/ModelTexture.h"

class Terrain {
public:
	Terrain(int gridX, int gridZ, Loader& loader, ModelTexture& texture);

private:
	RawModel* generateTerrain(Loader& loader);

	const float SIZE = 800;
	const int VERTEX_COUNT = 128;

	float m_x, m_z;
	RawModel* m_model;
	ModelTexture& m_texture;
};