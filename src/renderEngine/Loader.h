#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

#include "../models/RawModel.h"
#include "TextureData.h"

class Loader {
public:
	RawModel* loadToVAO(const std::vector<float>& positions,
						const std::vector<float>& texture_coords,
						const std::vector<float>& normals,
						const std::vector<unsigned int>& indices);

	RawModel* loadToVAO(const std::vector<float>& positions,
						const std::vector<float>& texture_coords,
						const std::vector<float>& normals,
						const std::vector<float>& tangents,
						const std::vector<unsigned int>& indices);

	unsigned int loadTexture(const std::string& fileName, float lodValue);
	unsigned int loadTexture(const std::string& fileName);

	unsigned int loadFontTexture(const std::string& fileName);

	int createEmptyVBO(std::vector<GLfloat>& data);
	void updateVBO(unsigned int vboID, std::vector<float>& data);

	int loadToVAO(std::vector<float>& positions, std::vector<float>& textureCoords);
	RawModel* loadToVAO(const std::vector<float> positions, int dimensions);

	void addInstanceAttribute(unsigned int vaoID, unsigned int vboID,
							  unsigned int attribute, int dataSize,
							  GLsizei instancedDataLength, int offset);



	TextureData* loadImage(std::string fileName);

	int loadCubeMap(std::vector<std::string> textureFiles);

	void cleanUp();

private:
	std::vector<unsigned int> vaos;
	std::vector<unsigned int> vbos;
	std::vector<unsigned int> textures;

	unsigned int createVAO();
	void unbindVAO();

	void storeDataInAttributeList(unsigned int attributeNumber,
								  unsigned int coordinateSize,
								  const float* data, unsigned int count);

	void bindIndicesBuffer(const unsigned int* indices, const unsigned int count);
};