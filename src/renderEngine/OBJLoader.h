#pragma once

#include "Vertex.h"
#include "../renderEngine/Loader.h"
#include "../models/RawModel.h"

namespace OBJLoader {
	void deal_with_already_processed_vertex(
		const std::shared_ptr<Vertex>& previous_vertex,
		int new_texture_index,
		int new_normal_index,
		std::vector<unsigned int>& indices,
		std::vector<std::shared_ptr<Vertex>>& vertices);

	void process_vertex(
		unsigned int index,
		unsigned int texture_index,
		unsigned int normal_index,
		std::vector<std::shared_ptr<Vertex>>& vertices,
		std::vector<unsigned int>& indices);

	float convert_data_to_arrays(
		const std::vector<std::shared_ptr<Vertex>>& vertices,
		const std::vector<glm::vec2>& textures,
		const std::vector<glm::vec3>& normals,
		std::vector<float>& vertices_array,
		std::vector<float>& textures_array,
		std::vector<float>& normals_array);

	void remove_unused_vertices(const std::vector<std::shared_ptr<Vertex>>& vertices);

	std::shared_ptr<RawModel> load_obj(
		const std::string& file_name,
		const std::shared_ptr<Loader>& loader);
}