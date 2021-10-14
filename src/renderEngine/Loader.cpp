#include "Loader.h"

RawModel* Loader::loadToVAO(const std::vector<float>& positions, const std::vector<float>& textureCoords,
	const std::vector<float>& normals, const std::vector<unsigned int>& indices) {
	unsigned int vaoID = createVAO();
	bindIndicesBuffer(indices.data(), indices.size());
	storeDataInAttributeList(0, 3, positions.data(), positions.size());
	storeDataInAttributeList(1, 2, textureCoords.data(), textureCoords.size());
	storeDataInAttributeList(2, 3, normals.data(), normals.size());
	unbindVAO();
	return new RawModel(vaoID, indices.size());
}

unsigned int Loader::loadTexture(const std::string& fileName) {
	unsigned int texture;
	glGenTextures(1, &texture);
	textures.push_back(texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, channels;
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if(image == NULL)
		std::cout << "Failed to load image!\n";

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

	stbi_image_free(image);

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