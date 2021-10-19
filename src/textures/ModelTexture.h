#pragma once

class ModelTexture {
public:
	ModelTexture(unsigned int id);

	void setShineDamper(float shineDamper) { m_shineDamper = shineDamper; };
	void setReflectivity(float reflectivity) { m_reflectivity = reflectivity; };

	inline unsigned int getID() { return m_textureID; };

	inline float& getShineDamper() { return m_shineDamper; };
	inline float& getReflectivity() { return m_reflectivity; };

	inline void setTransparency(bool value) { m_hasTransparency = value; };
	inline bool getTransparency() { return m_hasTransparency; };

	inline void useFakeLighting(bool value) { m_useFakeLighting = value; };
	inline bool isUsingFakeLighting() { return m_useFakeLighting; };

	inline void setNumberOfRows(int value) { m_numberOfRows = value; };
	inline bool getNumberOfRows() { return m_numberOfRows; };
private:
	unsigned int m_textureID;

	float m_shineDamper = 1;
	float m_reflectivity = 0;

	bool m_hasTransparency = false;
	bool m_useFakeLighting = false;

	int m_numberOfRows = 1;
};