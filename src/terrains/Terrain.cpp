#include "Terrain.h"

Terrain::Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap)
	: m_x(gridX* SIZE), m_z(gridZ* SIZE), m_texturePack(texturePack), m_blendMap(blendMap) {
	m_model = generateTerrain(loader, "res/textures/heightMap.png");
}

RawModel* Terrain::generateTerrain(Loader* loader, std::string heightMap) {
	int width, height, channels;
	unsigned char* image = stbi_load(heightMap.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if(image == NULL)
		std::cout << "Failed to load image!\n";

	TextureData* textureData = new TextureData(image, width, height);

	int VERTEX_COUNT = height;
	int count = VERTEX_COUNT * VERTEX_COUNT;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textureCoords;
	std::vector<unsigned int> indices;

	for(int i = 0; i < VERTEX_COUNT; i++) {
		for(int j = 0; j < VERTEX_COUNT; j++) {
			vertices.push_back((float)j / ((float)VERTEX_COUNT - 1) * SIZE);
			vertices.push_back(getHeight(j, i, textureData));
			vertices.push_back((float)i/((float)VERTEX_COUNT - 1) * SIZE);

			glm::vec3 normal = calculateNormal(j, i, textureData);
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
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

	//Clean up
	delete textureData;

	return loader->loadToVAO(vertices, textureCoords, normals, indices);
}

float Terrain::getHeight(int x, int z, TextureData* textureData) {
	if(x < 0 || x >= textureData->getWidth() || z < 0 || z >= textureData->getHeight())
		return 0;

	int index = 4 * (x + z * textureData->getWidth());
	unsigned char* data = textureData->getData();
	int r = *(data + index);
	int g = *(data + index + 1);
	int b = *(data + index + 2);

	float height = r + g + b;
	height -= MAX_PIXEL_COLOR / 2;
	height /= MAX_PIXEL_COLOR / 2;
	height *= MAX_HEIGHT;

	return height;
}

glm::vec3 Terrain::calculateNormal(int x, int z, TextureData* textureData) {
	float heightL = getHeight(x - 1, z, textureData);
	float heightR = getHeight(x + 1, z, textureData);
	float heightU = getHeight(x, z - 1, textureData);
	float heightD = getHeight(x, z + 1, textureData);

	glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	normal = glm::normalize(normal);

	return normal;
}