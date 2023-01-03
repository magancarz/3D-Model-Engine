#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "../models/RawModel.h"
#include "../renderEngine/Loader.h"
#include "VertexNM.h"

namespace NormalMappingOBJLoader {

	std::shared_ptr<VertexNM> process_vertex(
		int index,
		int texture_index,
		int normal_index,
		std::vector<std::shared_ptr<VertexNM>>& vertices,
		std::vector<unsigned int>& indices);

	void calculate_tangents(
		const std::shared_ptr<VertexNM>& v0,
		const std::shared_ptr<VertexNM>& v1,
		const std::shared_ptr<VertexNM>& v2,
		const std::vector<glm::vec2>& textures);

	float convert_data_to_arrays(
		const std::vector<std::shared_ptr<VertexNM>>& vertices,
		const std::vector<glm::vec2>& textures,
		const std::vector<glm::vec3>& normals,
		std::vector<float>& vertices_array,
		std::vector<float>& textures_array,
		std::vector<float>& normals_array,
		std::vector<float>& tangents_array);

	std::shared_ptr<VertexNM> deal_with_already_processed_vertex(
		std::shared_ptr<VertexNM> previous_vertex,
		int new_texture_index,
		int new_normal_index,
		std::vector<unsigned int>& indices,
		std::vector<std::shared_ptr<VertexNM>>& vertices);

	void remove_unused_vertices(std::vector<std::shared_ptr<VertexNM>>& vertices);

	std::shared_ptr<RawModel> load_normal_mapped_obj(
		const std::string& obj_file_name,
		const std::shared_ptr<Loader>& loader);
}