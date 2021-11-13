#pragma once

class ParticleTexture {
public:
	ParticleTexture(unsigned int textureID, unsigned int numberOfRows)
		: m_textureID(textureID), m_numberOfRows(numberOfRows) {}

	inline unsigned int getTextureID() { return m_textureID; }
	inline unsigned int getNumberOfRows() { return m_numberOfRows; }

private:
	unsigned int m_textureID,
				 m_numberOfRows;

};