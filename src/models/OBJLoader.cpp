#include "OBJLoader.h"

#include <glm/glm.hpp>

#include <sstream>
#include <fstream>
#include <ranges>

#include "Vertex.h"

void OBJLoader::deal_with_already_processed_vertex(
		const std::shared_ptr<Vertex>& previous_vertex,
		const int new_texture_index,
		const int new_normal_index,
		std::vector<unsigned int>& indices,
		std::vector<std::shared_ptr<Vertex>>& vertices) {

	if(previous_vertex->has_same_texture_and_normal(new_texture_index, new_normal_index)) {
		indices.push_back(previous_vertex->get_index());
	} else {
		const auto another_vertex = previous_vertex->get_duplicate_vertex();
		if(another_vertex != nullptr) {
			deal_with_already_processed_vertex(another_vertex, new_texture_index, new_normal_index, indices, vertices);
		} else {
			const auto duplicate_vertex = std::make_shared<Vertex>(vertices.size(), previous_vertex->get_position());
			duplicate_vertex->set_texture_index(new_texture_index);
			duplicate_vertex->set_normal_index(new_normal_index);
			previous_vertex->set_duplicate_vertex(duplicate_vertex);
			vertices.push_back(duplicate_vertex);
			indices.push_back(duplicate_vertex->get_index());
		}
	}
}

void OBJLoader::process_vertex(
		const unsigned int index,
		const unsigned int texture_index,
		const unsigned int normal_index,
		std::vector<std::shared_ptr<Vertex>>& vertices,
		std::vector<unsigned int>& indices) {

	const auto& current_vertex = vertices[index];
	if(!current_vertex->is_set()) {
		current_vertex->set_texture_index(texture_index);
		current_vertex->set_normal_index(normal_index);
		indices.push_back(index);
	} else {
		deal_with_already_processed_vertex(current_vertex, texture_index, normal_index, indices, vertices);
	}
}

float OBJLoader::convert_data_to_arrays(
		const std::vector<std::shared_ptr<Vertex>>& vertices,
		const std::vector<glm::vec2>& textures,
		const std::vector<glm::vec3>& normals,
		std::vector<float>& vertices_array,
		std::vector<float>& textures_array,
		std::vector<float>& normals_array) {

	float furthest_point = 0;

	for(const auto vertex : vertices) {
		if(vertex->get_length() > furthest_point)
			furthest_point = vertex->get_length();

		glm::vec3 position = vertex->get_position();
		glm::vec2 texture_coord = textures[vertex->get_texture_index()];
		glm::vec3 normal_vector = normals[vertex->get_normal_index()];

		vertices_array.push_back(position[0]);
		vertices_array.push_back(position[1]);
		vertices_array.push_back(position[2]);
		textures_array.push_back(texture_coord[0]);
		textures_array.push_back(1.0f - texture_coord[1]);
		normals_array.push_back(normal_vector[0]);
		normals_array.push_back(normal_vector[1]);
		normals_array.push_back(normal_vector[2]);
	}

	return furthest_point;
}

void OBJLoader::remove_unused_vertices(const std::vector<std::shared_ptr<Vertex>>& vertices) {
	for(auto& vertex : vertices) {
		if(!vertex->is_set()) {
			vertex->set_texture_index(0);
			vertex->set_normal_index(0);
		}
	}
}

std::shared_ptr<RawModel> OBJLoader::load_obj(const std::string& file_name, const std::shared_ptr<Loader>& loader) {
	std::ifstream in_file(file_name, std::ios::in);

	if(!in_file) {
		throw std::runtime_error("Unable to load obj file!\n");
	}

	std::string line;
	std::vector<std::shared_ptr<Vertex>> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	//This loop collects the vertices, texture coords and normals from the obj file.
	while(!in_file.eof()) {
		getline(in_file, line);

		std::istringstream iss(line);
		std::string starts;
		float x, y, z;

		//Starts contains e.g. v, vt, tv, s, f
		iss >> starts;

		if(starts == "v") {
			//e.g. v 3.227124 -0.065127 -1.000000
			iss >> x >> y >> z;
			glm::vec3 vertex(x, y, z);
			auto new_vertex = std::make_shared<Vertex>(vertices.size(), vertex);
			vertices.push_back(new_vertex);
		} else if(starts == "vt") {
			//e.g. vt 0.905299 0.932320
			iss >> x >> y;
			glm::vec2 texture(x, y);
			textures.push_back(texture);
		} else if(starts == "vn") {
			//e.g. vn -1.000000 0.000000 0.000000
			iss >> x >> y >> z;
			glm::vec3 normal(x, y, z);
			normals.push_back(normal);
		} else if(starts == "f") {
			//break when faces start
			break;
		}
	}

	std::vector<float> vertices_array;
	std::vector<float> normals_array;
	std::vector<float> texture_array;
	std::vector<unsigned int> indices_array;

	//read the faces in a second loop
	while(!in_file.eof()) {
		if(line.empty())
			break;

		std::istringstream iss(line);
		std::string starts;

		iss >> starts;

		if(starts == "f") {
			unsigned int u[9];
			//e.g. f 41/1/1 38/2/1 45/3/1
			std::string tmp, f;
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp;

			//replace /'s with space
			size_t x = f.find('/');
			while(x < std::string::npos) {
				f.replace(x, 1, " ");
				x = f.find('/', x + 1);
			}
			std::istringstream iss2(f);
			for(const int i : std::views::iota(0, 9)) {
				iss2 >> u[i];
				//the indices in the obj file start from 1, ours start from 0
				u[i]--;
			}

			//process vertices
			process_vertex(u[0], u[1], u[2], vertices, indices);
			process_vertex(u[3], u[4], u[5], vertices, indices);
			process_vertex(u[6], u[7], u[8], vertices, indices);
		}

		getline(in_file, line);
	}
	
	remove_unused_vertices(vertices);
	
	float furthest = convert_data_to_arrays(vertices, textures, normals, vertices_array, texture_array, normals_array);

	indices_array.resize(indices.size());
	for(const auto u : indices) {
		indices_array.push_back(u);
	}

	return loader->load_to_vao(vertices_array, texture_array, normals_array, indices_array);
}