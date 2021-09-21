#pragma once

#include "../Headers.h"

#include "../models/RawModel.h"

class Loader {
public:
	RawModel loadToVAO(const std::vector<float> positions, const std::vector<float> textureCoords, const std::vector<unsigned int> indices);
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