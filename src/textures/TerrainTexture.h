#pragma once

class TerrainTexture {
public:
	TerrainTexture(unsigned int texture_id);

	unsigned int get_texture_id() const;

private:
	unsigned int m_texture_id;
};