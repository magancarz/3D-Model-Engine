#include "Terrain.h"

#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include <iostream>

#include "toolbox/Maths.h"

Terrain::Terrain(
		const float grid_x,
		const float grid_z,
		const std::shared_ptr<Loader>& loader,
		std::shared_ptr<TerrainTexturePack> texture_pack,
		std::shared_ptr<TerrainTexture> blend_map) :
	m_x(grid_x * TERRAIN_SIZE),
	m_z(grid_z * TERRAIN_SIZE),
	m_texture_pack(std::move(texture_pack)),
	m_blend_map(std::move(blend_map)) {

	m_model = generate_terrain(loader, "res/textures/height_map.png");
}

Terrain::~Terrain() {
	for(int i = 0; i < m_heights_size; i++)
		delete m_heights[i];

	delete m_heights;
}

float Terrain::get_x() const { return m_x; }
float Terrain::get_z() const { return m_z; }

std::shared_ptr<RawModel> Terrain::get_model() const { return m_model; }

std::shared_ptr<TerrainTexturePack> Terrain::get_terrain_texture_pack() const { return m_texture_pack; }

std::shared_ptr<TerrainTexture> Terrain::get_blend_map() const { return m_blend_map; }

std::shared_ptr<RawModel> Terrain::generate_terrain(
		const std::shared_ptr<Loader>& loader,
		const std::string& height_map_location) {

	int width, height, channels;
	unsigned char* image = stbi_load(height_map_location.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if(image == nullptr)
		throw std::runtime_error("Failed to load image!\n");

	const auto texture_data = std::make_unique<TextureData>(image, width, height);

	const int vertex_count = m_heights_size = height;
	int count = vertex_count * vertex_count;

	m_heights = new float*[vertex_count];
	for(int i = 0; i < vertex_count; i++)
		m_heights[i] = new float[vertex_count];

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texture_coords;
	std::vector<unsigned int> indices;

	for(int i = 0; i < vertex_count; i++) {
		for(int j = 0; j < vertex_count; j++) {
			vertices.push_back(static_cast<float>(j) / (static_cast<float>(vertex_count) - 1.f) * TERRAIN_SIZE);
			float terrain_height = get_height(j, i, texture_data);
			m_heights[j][i] = terrain_height;
			vertices.push_back(terrain_height);
			vertices.push_back(static_cast<float>(i) / (static_cast<float>(vertex_count) - 1.f) * TERRAIN_SIZE);

			glm::vec3 normal = calculate_normal(j, i, texture_data);
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
			texture_coords.push_back(static_cast<float>(j) / (static_cast<float>(vertex_count) - 1.f));
			texture_coords.push_back(static_cast<float>(i) / (static_cast<float>(vertex_count) - 1.f));
		}
	}
	for(int gz = 0; gz < vertex_count - 1; gz++) {
		for(int gx = 0; gx < vertex_count - 1; gx++) {
			const int top_left = (gz * vertex_count) + gx;
			const int top_right = top_left + 1;
			const int bottom_left = ((gz + 1) * vertex_count) + gx;
			const int bottom_right = bottom_left + 1;
			indices.push_back(top_left);
			indices.push_back(bottom_left);
			indices.push_back(top_right);
			indices.push_back(top_right);
			indices.push_back(bottom_left);
			indices.push_back(bottom_right);
		}
	}

	stbi_image_free(image);

	return loader->load_to_vao(vertices, texture_coords, normals, indices);
}

float Terrain::get_height(const int x, const int z, const std::unique_ptr<TextureData>& texture_data) const {
	if(x < 0 || x >= texture_data->get_width() || z < 0 || z >= texture_data->get_height())
		return 0;

	const int index = 4 * (x + z * texture_data->get_width());
	const unsigned char* data = texture_data->get_data();
	const float r = *(data + index);
	const float g = *(data + index + 1);
	const float b = *(data + index + 2);

	float height = r + g + b;
	height -= MAX_PIXEL_COLOR / 2;
	height /= MAX_PIXEL_COLOR / 2;
	height *= TERRAIN_MAX_HEIGHT;

	return height;
}

glm::vec3 Terrain::calculate_normal(const int x, const int z, const std::unique_ptr<TextureData>& texture_data) const {
	const float height_l = get_height(x - 1, z, texture_data);
	const float height_r = get_height(x + 1, z, texture_data);
	const float height_u = get_height(x, z - 1, texture_data);
	const float height_d = get_height(x, z + 1, texture_data);

	glm::vec3 normal(height_l - height_r, 2.0f, height_d - height_u);
	normal = glm::normalize(normal);

	return normal;
}

float Terrain::get_height_of_terrain(const float world_x, const float world_z) const {
	const float terrain_x = world_x - m_x;
	const float terrain_z = world_z - m_z;
	float grid_square_size = TERRAIN_SIZE / (static_cast<float>(m_heights_size) - 1);
	const int grid_x = static_cast<int>(glm::floor(terrain_x / grid_square_size));
	const int grid_z = static_cast<int>(glm::floor(terrain_z / grid_square_size));

	if(grid_x >= m_heights_size - 1 || grid_z >= m_heights_size - 1 || grid_x < 0 || grid_z < 0) 
		return 0;

	float x_coord = glm::modf(terrain_x, grid_square_size) / grid_square_size;
	float z_coord = glm::modf(terrain_z, grid_square_size) / grid_square_size;

	float answer;
	if(x_coord <= (1 - z_coord)) {
		answer = static_cast<float>(barycentric(glm::vec3(0, m_heights[grid_x][grid_z], 0),
							 glm::vec3(1, m_heights[grid_x + 1][grid_z], 0),
							 glm::vec3(0, m_heights[grid_x][grid_z + 1], 1),
							 glm::vec2(x_coord, z_coord)));
	} else {
		answer = static_cast<float>(barycentric(glm::vec3(1, m_heights[grid_x + 1][grid_z], 0),
							 glm::vec3(1, m_heights[grid_x + 1][grid_z + 1], 0),
							 glm::vec3(0, m_heights[grid_x][grid_z + 1], 1),
							 glm::vec2(x_coord, z_coord)));
	}

	return answer;
}