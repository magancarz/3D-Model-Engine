#pragma once

class ModelTexture {
public:
	ModelTexture(unsigned int id);

	void setShineDamper(float shineDamper) { m_shineDamper = shineDamper; };
	void setReflectivity(float reflectivity) { m_reflectivity = reflectivity; };

	inline unsigned int getID() { return m_textureID; };

	inline float& getShineDamper() { return m_shineDamper; };
	inline float& getReflectivity() { return m_reflectivity; };
private:
	unsigned int m_textureID;

	float m_shineDamper = 1;
	float m_reflectivity = 0;
};