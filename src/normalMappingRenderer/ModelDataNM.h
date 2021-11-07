#pragma once

#include "../Headers.h"

class ModelDataNM {
public:
	ModelDataNM(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals, std::vector<float> tangents, std::vector<int> indices, float furthestPoint)
		: m_vertices(vertices), m_textureCoords(textureCoords), m_normals(normals), m_tangents(tangents), m_indices(indices), m_furthestPoint(furthestPoint) {}

private:
	std::vector<float> m_vertices,
					   m_textureCoords,
					   m_normals,
					   m_tangents;
	std::vector<int> m_indices;
	float m_furthestPoint;

};