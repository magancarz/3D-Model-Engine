#include "Loader.h"

RawModel* Loader::loadToVAO(const std::vector<float>& positions,
							const std::vector<float>& textureCoords,
							const std::vector<float>& normals,
							const std::vector<unsigned int>& indices) {
	unsigned int vaoID = createVAO();
	bindIndicesBuffer(indices.data(), indices.size());
	storeDataInAttributeList(0, 3, positions.data(), positions.size());
	storeDataInAttributeList(1, 2, textureCoords.data(), textureCoords.size());
	storeDataInAttributeList(2, 3, normals.data(), normals.size());
	unbindVAO();

	return new RawModel(vaoID, indices.size());
}

RawModel* Loader::loadToVAO(const std::vector<float>& positions, const std::vector<float>& textureCoords,
	const std::vector<float>& normals, const std::vector<float>& tangents, const std::vector<unsigned int>& indices) {
	unsigned int vaoID = createVAO();
	bindIndicesBuffer(indices.data(), indices.size());
	storeDataInAttributeList(0, 3, positions.data(), positions.size());
	storeDataInAttributeList(1, 2, textureCoords.data(), textureCoords.size());
	storeDataInAttributeList(2, 3, normals.data(), normals.size());
	storeDataInAttributeList(3, 3, tangents.data(), tangents.size());
	unbindVAO();

	return new RawModel(vaoID, indices.size());
}

int Loader::loadToVAO(std::vector<float>& positions, std::vector<float>& textureCoords) {
	unsigned int vaoID = createVAO();
	storeDataInAttributeList(0, 2, positions.data(), positions.size());
	storeDataInAttributeList(1, 2, textureCoords.data(), textureCoords.size());
	unbindVAO();

	return vaoID;
}

int Loader::createEmptyVBO(std::vector<GLfloat>& data) {
	unsigned int vbo = 0;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

void Loader::addInstanceAttribute(unsigned int vaoID, unsigned int vboID, unsigned int attribute, int dataSize, GLsizei instancedDataLength, int offset) {
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBindVertexArray(vaoID);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, instancedDataLength * sizeof(float), (const void*)(offset * sizeof(float)));
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Loader::updateVBO(unsigned int vboID, std::vector<float>& data) {
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), &data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

RawModel* Loader::loadToVAO(const std::vector<float> positions, int dimensions) {
	int vaoID = createVAO();
	storeDataInAttributeList(0, dimensions, positions.data(), positions.size());
	unbindVAO();

	return new RawModel(vaoID, positions.size() / dimensions);
}

unsigned int Loader::loadTexture(const std::string& fileName, float lodValue) {
	unsigned int texture;
	glGenTextures(1, &texture);
	textures.push_back(texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	TextureData* data = loadImage(fileName);
	int width = data->getWidth(),
		height = data->getHeight();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lodValue);
	
	// check if anisotropic filtering is supported
	int no_of_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &no_of_extensions);

	std::set<std::string> ogl_extensions;
	for(int i = 0; i < no_of_extensions; i++) {
		ogl_extensions.insert((const char*) glGetStringi(GL_EXTENSIONS, i));
	}

	bool anisotropicFilteringEnabled = ogl_extensions.find("GL_EXT_texture_filter_anisotropic") != ogl_extensions.end();
	if(anisotropicFilteringEnabled) {
		float maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		float amount = std::min(4.0f, maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
	} else {
		std::cout << "WARNING: Anisotropic filtering is not supported\n";
	}

	//Clean up
	stbi_image_free(data->getData());
	delete data;

	return texture;
}

unsigned int Loader::loadTexture(const std::string& fileName) {
	return loadTexture(fileName, -0.4);
}

unsigned int Loader::loadFontTexture(const std::string& fileName) {
	return loadTexture(fileName, 0.0);
}

//!!!Remember to use stbi_image_free after using data!!!
TextureData* Loader::loadImage(std::string fileName) {
	int width, height, format;
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &format, STBI_rgb_alpha);
	if(image == NULL)
		std::cout << "Failed to load image!\n";
	
	return new TextureData(image, width, height);
}

int Loader::loadCubeMap(std::vector<std::string> textureFiles) {
	unsigned int texID; 
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	//Texture files size should be always 6
	for(int i = 0; i < textureFiles.size(); i++) {
		int width, height, format;
		TextureData* data = loadImage("res/textures/" + textureFiles[i] + ".png");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, data->getWidth(), data->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data->getData());

		//Clean up
		stbi_image_free(data->getData());
		delete data;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	textures.push_back(texID);

	return texID;
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