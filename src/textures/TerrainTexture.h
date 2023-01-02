#pragma once

class TerrainTexture {
public:
	TerrainTexture(const unsigned int texture_id) :
	m_texture_id(texture_id) {}

	unsigned int get_texture_id() const { return m_texture_id; }

private:
	unsigned int m_texture_id;
};