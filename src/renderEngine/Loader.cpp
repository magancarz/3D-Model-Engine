#include "Loader.h"

#include <iostream>

RawModel Loader::loadToVAO(const float* positions, const unsigned int positionsCount,
	const float* textureCoords, unsigned int textureCoordsCount,
	const unsigned int* indices, const unsigned int indicesCount) {
	unsigned int vaoID = createVAO();
	bindIndicesBuffer(indices, indicesCount);
	storeDataInAttributeList(0, 3, positions, positionsCount);
	storeDataInAttributeList(1, 2, textureCoords, textureCoordsCount);
	unbindVAO();
	RawModel model(vaoID, indicesCount);
	return model;
}

unsigned int Loader::loadTexture(const std::string& fileName) {
	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, channels;
	glBindTexture(GL_TEXTURE_2D, texture);
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
	if(image == NULL)
		std::cout << "Failed to load image!\n";
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

void Loader::cleanUp() {
	for(auto& vao : vaos) {
		glDeleteVertexArrays(1, &vao);
	}
	for(auto& vbo : vbos) {
		glDeleteBuffers(1, &vbo);
	}
	for(auto& texture : textures) {
		glDeleteTextures(1, &texture);
	}
}

unsigned int Loader::createVAO() {
	unsigned int vaoID;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void Loader::storeDataInAttributeList(unsigned int attributeNumber, unsigned int coordinateSize, const float* data, unsigned int count) {
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
}

void Loader::unbindVAO() {
	glBindVertexArray(0);
}

void Loader::bindIndicesBuffer(const unsigned int* indices, const unsigned int count) {
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}