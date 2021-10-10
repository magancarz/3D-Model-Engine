#pragma once

class TerrainTexture {
public:
	TerrainTexture(unsigned int textureID)
		: m_textureID(textureID) {}

	inline unsigned int getTextureID() { return m_textureID; };
private:
	unsigned int m_textureID;
};