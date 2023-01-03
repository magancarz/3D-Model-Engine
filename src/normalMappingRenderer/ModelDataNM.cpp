#include "ModelDataNM.h"

ModelDataNM::ModelDataNM(
		std::vector<float> vertices,
		std::vector<float> texture_coords,
		std::vector<float> normals,
		std::vector<float> tangents,
		std::vector<int> indices,
		const float furthest_point) :
m_vertices(std::move(vertices)),
m_texture_coords(std::move(texture_coords)),
m_normals(std::move(normals)),
m_tangents(std::move(tangents)),
m_indices(std::move(indices)),
m_furthest_point((furthest_point)) {}