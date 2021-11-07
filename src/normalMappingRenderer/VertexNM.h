#pragma once

#include "../Headers.h"

class VertexNM {
public:
	VertexNM(int index, glm::vec3 position);
	~VertexNM();
	void addTangent(glm::vec3 tangent);
	VertexNM* duplicate(int newIndex);
	void averageTangents();
	
	inline glm::vec3 getAverageTangent() { return m_averagedTangent; };
	inline int getIndex() { return m_index; };
	inline int getLength() { return m_length; };
	inline bool isSet() { return m_textureIndex != NO_INDEX && m_normalIndex != NO_INDEX; };
	inline bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther) { return textureIndexOther == m_textureIndex && normalIndexOther == m_normalIndex; };
	glm::vec3 getPosition() { return m_position; };
	int getTextureIndex() { return m_textureIndex; };
	int getNormalIndex() { return m_normalIndex; };
	VertexNM* getDuplicateVertex() { return m_duplicateVertex; };

	void setTextureIndex(int textureIndex) { m_textureIndex = textureIndex; };
	void setNormalIndex(int normalIndex) { m_normalIndex = normalIndex; };
	void setDuplicateVertex(VertexNM* duplicateVertex) { m_duplicateVertex = duplicateVertex; };

private:
	static constexpr int NO_INDEX = -1;

	glm::vec3 m_position;
	int m_textureIndex = NO_INDEX;
	int m_normalIndex = NO_INDEX;
	VertexNM* m_duplicateVertex = nullptr;
	int m_index;
	float m_length;
	std::vector<glm::vec3> m_tangents;
	glm::vec3 m_averagedTangent;
};