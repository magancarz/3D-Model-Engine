#pragma once

#include <memory>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "../models/RawModel.h"
#include "TextureData.h"

namespace locations {
	inline std::string res_folder_location = "res/";
    inline std::string textures_folder_location = "res/textures/";
    inline std::string image_extension = ".png";
	inline std::string models_folder_location = "res/models/";
    inline std::string model_extension = ".obj";
    inline std::string shader_extension = ".glsl";
    inline std::string shader_folder_location = "res/shaders/";
}

class Loader {
public:
	~Loader();

	std::shared_ptr<RawModel> load_to_vao(
		const std::vector<float>& positions,
		int dimensions);

	std::shared_ptr<RawModel> load_to_vao(
		const std::vector<float>& positions,
		const std::vector<float>& texture_coords,
		const std::vector<float>& normals,
		const std::vector<unsigned int>& indices);

	std::shared_ptr<RawModel> load_to_vao(
		const std::vector<float>& positions,
		const std::vector<float>& texture_coords,
		const std::vector<float>& normals,
		const std::vector<float>& tangents,
		const std::vector<unsigned int>& indices);

	unsigned int load_to_vao(
		const std::vector<float>& positions,
		const std::vector<float>& texture_coords);

	static std::shared_ptr<TextureData> load_image(const std::string& file_name);

	unsigned int load_texture(const std::string& file_name, float lod_value);

	unsigned int load_texture(const std::string& file_name);

	unsigned int load_font_texture(const std::string& file_name);

	unsigned int load_cube_map(const std::vector<std::string>& texture_files);

	unsigned int create_empty_vbo(const std::vector<float>& data);

	static void update_vbo(unsigned int vbo_id, const std::vector<float>& data);

	static void add_instance_attribute(
		unsigned int vao_id,
		unsigned int vbo_id,
		unsigned int attribute,
		int data_size,
		int instanced_data_length,
		int offset);

private:
	std::vector<unsigned int> vaos;
	std::vector<unsigned int> vbos;
	std::vector<unsigned int> textures;

	unsigned int create_vao();
	static void unbind_vao();

	void store_data_in_attribute_list(
		unsigned int attribute_number,
		unsigned int coordinate_size,
		const float* data,
		unsigned int count);

	void bind_indices_buffer(const unsigned int* indices, unsigned int count);
};