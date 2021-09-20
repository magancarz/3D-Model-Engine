#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <stb_image/stb_image.h>

#include "../models/RawModel.h"

class Loader {
public:
	RawModel loadToVAO(const float* positions, const unsigned int positionsCount,
		const float* textureCoords, unsigned int textureCoordsCount,
		const unsigned int* indices, const unsigned int indicesCount);
	unsigned int loadTexture(const std::string& fileName);
	void cleanUp();
private:
	std::vector<unsigned int> vaos;
	std::vector<unsigned int> vbos;
	std::vector<unsigned int> textures;

	unsigned int createVAO();
	void storeDataInAttributeList(unsigned int attributeNumber, unsigned int coordinateSize, const float* data, unsigned int count);
	void unbindVAO();
	void bindIndicesBuffer(const unsigned int* indices, const unsigned int count);
};