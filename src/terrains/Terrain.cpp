#include "Terrain.h"

Terrain::Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap)
	: m_x(gridX * SIZE), m_z(gridZ * SIZE), m_texturePack(texturePack), m_blendMap(blendMap) {
	m_model = generateTerrain(loader);
}

RawModel* Terrain::generateTerrain(Loader* loader) {
	int count = VERTEX_COUNT * VERTEX_COUNT;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textureCoords;
	std::vector<unsigned int> indices;

	for(int i = 0; i < VERTEX_COUNT; i++) {
		for(int j = 0; j < VERTEX_COUNT; j++) {
			vertices.push_back((float)j / ((float)VERTEX_COUNT - 1) * SIZE);
			vertices.push_back(0);
			vertices.push_back((float)i/((float)VERTEX_COUNT - 1) * SIZE);
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
			textureCoords.push_back((float)j / ((float)VERTEX_COUNT - 1));
			textureCoords.push_back((float)i / ((float)VERTEX_COUNT - 1));
		}
	}
	for(int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
		for(int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	return loader->loadToVAO(vertices, textureCoords, normals, indices);
}