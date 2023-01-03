#pragma once

#include <vector>

class ModelData {
public:
	ModelData(std::vector<float>* vertices, std::vector<float>* textureCoords, std::vector<float>* normals, std::vector<int>* indices, float furthestPoint)
		: m_vertices(vertices), m_textureCoords(textureCoords), m_normals(normals), m_indices(indices), m_furthestPoint(furthestPoint) {}

	inline std::vector<float>* getVertices() { return m_vertices; }
	inline std::vector<float>* getTextureCoords() { return m_textureCoords; }
	inline std::vector<float>* getNormals() { return m_normals; }
	inline std::vector<int>* getIndices() { return m_indices; }
	inline float getFurthestPoint() { return m_furthestPoint; }
private:
	std::vector<float>* m_vertices;
	std::vector<float>* m_textureCoords;
	std::vector<float>* m_normals;
	std::vector<int>* m_indices;
	float m_furthestPoint;
};
