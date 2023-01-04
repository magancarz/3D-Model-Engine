#include "TerrainTexture.h"

TerrainTexture::TerrainTexture(const unsigned int texture_id) :
m_texture_id(texture_id) {}

unsigned int TerrainTexture::get_texture_id() const {
	return m_texture_id;
}