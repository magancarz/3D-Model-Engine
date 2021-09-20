#pragma once

class RawModel {
public:
	RawModel(unsigned int vaoID, unsigned int vertexCount);

	inline unsigned int getVaoID() const { return m_vaoID; };
	inline unsigned int getVertexCount() const { return m_vertexCount; };
private:
	unsigned int m_vaoID;
	unsigned int m_vertexCount;
};