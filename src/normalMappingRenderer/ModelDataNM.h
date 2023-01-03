#pragma once

#include <vector>

class ModelDataNM {
public:
	ModelDataNM(
		std::vector<float> vertices,
		std::vector<float> texture_coords,
		std::vector<float> normals,
		std::vector<float> tangents,
		std::vector<int> indices,
		float furthest_point);

private:
	std::vector<float> m_vertices,
					   m_texture_coords,
					   m_normals,
					   m_tangents;

	std::vector<int> m_indices;

	float m_furthest_point;

};