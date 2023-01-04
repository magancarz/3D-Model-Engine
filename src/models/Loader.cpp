#include "Loader.h"

#include <iostream>
#include <set>
#include <ranges>

#include "stb_image/stb_image.h"

Loader::~Loader() {
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


std::shared_ptr<RawModel> Loader::load_to_vao(
		const std::vector<float>& positions,
		const int dimensions) {

	const unsigned int vao_id = create_vao();
	store_data_in_attribute_list(0, dimensions, positions.data(), positions.size());
	unbind_vao();

	return std::make_shared<RawModel>(vao_id, positions.size() / dimensions);
}

std::shared_ptr<RawModel> Loader::load_to_vao(
		const std::vector<float>& positions,
		const std::vector<float>& texture_coords,
		const std::vector<float>& normals,
		const std::vector<unsigned int>& indices) {

	const unsigned int vao_id = create_vao();
	bind_indices_buffer(indices.data(), indices.size());
	store_data_in_attribute_list(0, 3, positions.data(), positions.size());
	store_data_in_attribute_list(1, 2, texture_coords.data(), texture_coords.size());
	store_data_in_attribute_list(2, 3, normals.data(), normals.size());
	unbind_vao();

	return std::make_shared<RawModel>(vao_id, indices.size());
}

std::shared_ptr<RawModel> Loader::load_to_vao(
		const std::vector<float>& positions,
		const std::vector<float>& texture_coords,
		const std::vector<float>& normals,
		const std::vector<float>& tangents,
		const std::vector<unsigned int>& indices) {

	const unsigned int vao_id = create_vao();
	bind_indices_buffer(indices.data(), indices.size());
	store_data_in_attribute_list(0, 3, positions.data(), positions.size());
	store_data_in_attribute_list(1, 2, texture_coords.data(), texture_coords.size());
	store_data_in_attribute_list(2, 3, normals.data(), normals.size());
	store_data_in_attribute_list(3, 3, tangents.data(), tangents.size());
	unbind_vao();

	return std::make_shared<RawModel>(vao_id, indices.size());
}

unsigned int Loader::load_to_vao(
		const std::vector<float>& positions,
		const std::vector<float>& texture_coords) {

	const unsigned int vao_id = create_vao();
	store_data_in_attribute_list(0, 2, positions.data(), positions.size());
	store_data_in_attribute_list(1, 2, texture_coords.data(), texture_coords.size());
	unbind_vao();

	return vao_id;
}

std::shared_ptr<TextureData> Loader::load_image(const std::string& file_name) {
	const std::string file = locations::textures_folder_location + file_name + locations::image_extension;
	int width, height, format;
	unsigned char* image = stbi_load(file.c_str(), &width, &height, &format, STBI_rgb_alpha);
	if(image == nullptr)
		std::cout << "Failed to load image!\n";
	
	return std::make_shared<TextureData>(image, width, height);
}

unsigned int Loader::load_texture(const std::string& file_name, const float lod_value) {
	unsigned int texture;
	glGenTextures(1, &texture);
	textures.push_back(texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	const auto data = load_image(file_name);
	const int width  = data->get_width(),
			  height = data->get_height();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->get_data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod_value);
	
	// check if anisotropic filtering is supported
	int no_of_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &no_of_extensions);

	std::set<std::string> ogl_extensions;
	for(int i = 0; i < no_of_extensions; i++) {
		ogl_extensions.insert(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
	}

	if(ogl_extensions.contains("GL_EXT_texture_filter_anisotropic")) {
		float max_aniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
		const float amount = std::min(4.0f, max_aniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
	} else {
		std::cout << "WARNING: Anisotropic filtering is not supported\n";
	}
	
	stbi_image_free(data->get_data());

	return texture;
}

unsigned int Loader::load_texture(const std::string& file_name) {
	return load_texture(file_name, -0.4f);
}

unsigned int Loader::load_font_texture(const std::string& file_name) {
	return load_texture(file_name, 0.0);
}

unsigned int Loader::load_cube_map(const std::vector<std::string>& texture_files) {
	unsigned int tex_id; 
	glGenTextures(1, &tex_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);

	//Texture files size should be always 6
	for(const int i : std::views::iota(0, 6)) {
		int width, height, format;
		const auto data = load_image(texture_files[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, data->get_width(), data->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data->get_data());
		
		stbi_image_free(data->get_data());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	textures.push_back(tex_id);

	return tex_id;
}

unsigned int Loader::create_empty_vbo(const std::vector<GLfloat>& data) {
	unsigned int vbo = 0;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

void Loader::update_vbo(const unsigned int vbo_id, const std::vector<float>& data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), &data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::add_instance_attribute(
		const unsigned int vao_id,
		const unsigned int vbo_id,
		const unsigned int attribute,
		const int data_size,
		const int instanced_data_length,
		const int offset) {

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBindVertexArray(vao_id);
	glVertexAttribPointer(attribute, data_size, GL_FLOAT, GL_FALSE, instanced_data_length * sizeof(float), reinterpret_cast<const void*>(offset * sizeof(float)));
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int Loader::create_vao() {
	unsigned int vao_id;
	glGenVertexArrays(1, &vao_id);
	vaos.push_back(vao_id);
	glBindVertexArray(vao_id);

	return vao_id;
}

void Loader::unbind_vao() {
	glBindVertexArray(0);
}

void Loader::store_data_in_attribute_list(
		const unsigned int attribute_number,
		const unsigned int coordinate_size,
		const float* data,
		const unsigned int count) {

	unsigned int vbo_id;
	glGenBuffers(1, &vbo_id);
	vbos.push_back(vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attribute_number, coordinate_size, GL_FLOAT, false, 0, 0);
}

void Loader::bind_indices_buffer(const unsigned int* indices, const unsigned int count) {
	unsigned int vbo_id;
	glGenBuffers(1, &vbo_id);
	vbos.push_back(vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}