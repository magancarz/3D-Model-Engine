#include "NormalMappingObjLoader.h"

#include <sstream>
#include <fstream>
#include <ranges>

std::shared_ptr<VertexNM> NormalMappingOBJLoader::process_vertex(
		const int index,
		const int texture_index,
		const int normal_index,
		std::vector<std::shared_ptr<VertexNM>>& vertices,
		std::vector<unsigned int>& indices) {

	auto current_vertex = vertices[index];
	if (!current_vertex->is_set()) {
		current_vertex->set_texture_index(texture_index);
		current_vertex->set_normal_index(normal_index);
		indices.push_back(index);
		return current_vertex;
	}

	return NormalMappingOBJLoader::deal_with_already_processed_vertex(
		current_vertex,
		texture_index,
		normal_index,
		indices,
		vertices);
}

void NormalMappingOBJLoader::calculate_tangents(
		const std::shared_ptr<VertexNM>& v0,
		const std::shared_ptr<VertexNM>& v1,
		const std::shared_ptr<VertexNM>& v2,
		const std::vector<glm::vec2>& textures) {

	glm::vec3 delta_pos1 = v1->get_position() - v0->get_position();
	glm::vec3 delta_pos2 = v2->get_position() - v0->get_position();
	const glm::vec2 uv0 = textures[v0->get_texture_index()];
	const glm::vec2 uv1 = textures[v1->get_texture_index()];
	const glm::vec2 uv2 = textures[v2->get_texture_index()];
	const glm::vec2 delta_uv1 = uv1 - uv0;
	const glm::vec2 delta_uv2 = uv2 - uv0;

	const float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
	delta_pos1 *= delta_uv2.y;
	delta_pos2 *= delta_uv1.y;
	glm::vec3 tangent = delta_pos1 - delta_pos2;
	tangent *= r;
	v0->add_tangent(tangent);
	v1->add_tangent(tangent);
	v2->add_tangent(tangent);
}

float NormalMappingOBJLoader::convert_data_to_arrays(
		const std::vector<std::shared_ptr<VertexNM>>& vertices,
		const std::vector<glm::vec2>& textures,
		const std::vector<glm::vec3>& normals,
		std::vector<float>& vertices_array,
		std::vector<float>& textures_array,
		std::vector<float>& normals_array,
		std::vector<float>& tangents_array) {

	float furthest_point = 0;

	for(const auto current_vertex : vertices) {
		if (current_vertex->get_length() > furthest_point) {
			furthest_point = current_vertex->get_length();
		}
		glm::vec3 position = current_vertex->get_position();
		glm::vec2 texture_coord = textures[current_vertex->get_texture_index()];
		glm::vec3 normal_vector = normals[current_vertex->get_normal_index()];
		glm::vec3 tangent = current_vertex->get_average_tangent();

		vertices_array.push_back(position.x);
		vertices_array.push_back(position.y);
		vertices_array.push_back(position.z);
		textures_array.push_back(texture_coord.x);
		textures_array.push_back(1.0 - texture_coord.y);
		normals_array.push_back(normal_vector.x);
		normals_array.push_back(normal_vector.y);
		normals_array.push_back(normal_vector.z);
		tangents_array.push_back(tangent.x);
		tangents_array.push_back(tangent.y);
		tangents_array.push_back(tangent.z);
	}
	return furthest_point;
}

std::shared_ptr<VertexNM> NormalMappingOBJLoader::deal_with_already_processed_vertex(
		std::shared_ptr<VertexNM> previous_vertex,
		int new_texture_index,
		int new_normal_index,
		std::vector<unsigned int>& indices,
		std::vector<std::shared_ptr<VertexNM>>& vertices) {

	if(previous_vertex->has_same_texture_and_normal(new_texture_index, new_normal_index)) {
		indices.push_back(previous_vertex->get_index());
		return previous_vertex;
	}

	const auto vertex = previous_vertex->get_duplicate_vertex();
	if (vertex != nullptr) {
		return deal_with_already_processed_vertex(vertex, new_texture_index, new_normal_index,
			indices, vertices);
	}

	auto duplicate_vertex = std::make_shared<VertexNM>(vertices.size(), previous_vertex->get_position());
	duplicate_vertex->set_texture_index(new_texture_index);
	duplicate_vertex->set_normal_index(new_normal_index);
	previous_vertex->set_duplicate_vertex(duplicate_vertex);
	vertices.push_back(duplicate_vertex);
	indices.push_back(duplicate_vertex->get_index());

	return duplicate_vertex;
}

void NormalMappingOBJLoader::remove_unused_vertices(std::vector<std::shared_ptr<VertexNM>>& vertices) {
	for(const auto& vertex : vertices) {
		vertex->average_tangents();
		if (!vertex->is_set()) {
			vertex->set_texture_index(0);
			vertex->set_normal_index(0);
		}
	}
}

std::shared_ptr<RawModel> NormalMappingOBJLoader::load_normal_mapped_obj(
		const std::string& obj_file_name,
		const std::shared_ptr<Loader>& loader) {

	std::ifstream in_file(obj_file_name, std::ios::in);

	if(!in_file) {
		throw std::runtime_error("Unable to load obj file!\n");
	}

	std::string line;
	std::vector<std::shared_ptr<VertexNM>> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<unsigned int> indices;

	// This loop collects the vertices, texture coords and normals from
	// the obj file.
	while(!in_file.eof()) {
		getline(in_file, line);

		std::istringstream iss(line);
		std::string starts;
		float x, y, z;

		// starts contains e.g. v, vt, tv, s, f
		iss >> starts;

		if (starts == "v") {
			// e.g. v 3.227124 -0.065127 -1.000000
			iss >> x >> y >> z;
			glm::vec3 vertex(x, y, z);
			auto new_vertex = std::make_shared<VertexNM>(vertices.size(), vertex);
			vertices.push_back(new_vertex);
		} else if (starts == "vt") {
			// e.g. vt 0.905299 0.942320
			iss >> x >> y;
			glm::vec2 texture(x, y);
			textures.push_back(texture);
		} else if (starts == "vn") {
			// e.g. vn -1.000000 0.000000 0.000000
			iss >> x >> y >> z;
			glm::vec3 normal(x, y, z);
			normals.push_back(normal);
		} else if (starts == "f") {
			// break when faces start
			break;
		}
	}

	int faces = 0;

	// read the faces in a second loop
	while(!in_file.eof() ) {
		if(line.empty())
			break;

		std::istringstream iss(line);
		std::string starts;

		iss >> starts;

		if (starts == "f") {
			unsigned int u[9];
			// e.g. f 41/1/1 38/2/1 45/3/1
			std::string tmp, f;
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp;

			// replace /'s with space.
			size_t x = f.find('/');
			while (x < std::string::npos) {
				f.replace (x, 1, " ");
				x = f.find('/', x + 1);
			}
			std::istringstream iss2(f);
			for(const int i : std::views::iota(0, 9)) {
				iss2 >> u[i];
				// the indices in the obj file start from 1, ours start from 0
				u[i]--;
			}

			auto v0 = NormalMappingOBJLoader::process_vertex(u[0], u[1], u[2], vertices, indices);
			auto v1 = NormalMappingOBJLoader::process_vertex(u[3], u[4], u[5], vertices, indices);
			auto v2 = NormalMappingOBJLoader::process_vertex(u[6], u[7], u[8], vertices, indices);
			calculate_tangents(v0, v1, v2, textures);

			faces++;
		}

		getline(in_file, line);
	}

	remove_unused_vertices(vertices);

	std::vector<float> vertices_array;
	std::vector<float> textures_array;
	std::vector<float> normals_array;
	std::vector<float> tangents_array;

	float furthest = convert_data_to_arrays(
		vertices, textures, normals,
		vertices_array, textures_array, normals_array, tangents_array);

	std::vector<unsigned int> indices_array;

	indices_array.reserve(indices.size());
	for(const auto u : indices) {
		indices_array.push_back(u);
	}

	return loader->load_to_vao(vertices_array, textures_array, normals_array,
				tangents_array, indices_array);
}