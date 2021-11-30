#pragma once

#include "../Headers.h"

class Vertex {
public:
	Vertex(int index, glm::vec3 position)
		: m_index(index), m_position(position), m_length(position.length()) {}

	inline int getIndex() { return m_index; };
	inline float getLength() { return m_length; };
	inline bool isSet() { return m_textureIndex != NO_INDEX && m_normalIndex != NO_INDEX; };
	inline bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther) { return textureIndexOther == m_textureIndex && normalIndexOther == m_normalIndex; };

	glm::vec3 getPosition() { return m_position; };
	int getTextureIndex() { return m_textureIndex; };
	int getNormalIndex() { return m_normalIndex; };
	Vertex* getDuplicateVertex() { return m_duplicateVertex; };

	void setTextureIndex(int textureIndex) { m_textureIndex = textureIndex; };
	void setNormalIndex(int normalIndex) { m_normalIndex = normalIndex; };

	void setDuplicateVertex(Vertex* duplicateVertex) { m_duplicateVertex = duplicateVertex; };
private:
	const int NO_INDEX = -1;

	glm::vec3 m_position;
	int m_textureIndex = NO_INDEX;
	int m_normalIndex = NO_INDEX;
	Vertex* m_duplicateVertex = nullptr;
	int m_index;
	float m_length;
};