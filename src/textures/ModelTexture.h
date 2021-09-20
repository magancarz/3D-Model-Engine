#pragma once

class ModelTexture {
public:
	ModelTexture(unsigned int id);

	inline unsigned int getID() { return m_textureID; };
private:
	unsigned int m_textureID;
};