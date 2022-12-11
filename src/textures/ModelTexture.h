#pragma once

class ModelTexture {
public:
	ModelTexture(unsigned int id);

	void setShineDamper(float shineDamper) { m_shineDamper = shineDamper; }
	void setReflectivity(float reflectivity) { m_reflectivity = reflectivity; }

	inline unsigned int getID() { return m_textureID; }

	inline void setNormalMap(int value) { m_normalMap = value; }
	inline int getNormalMap() { return m_normalMap; }

	inline float& getShineDamper() { return m_shineDamper; }
	inline float& getReflectivity() { return m_reflectivity; }

	inline void setTransparency(bool value) { m_hasTransparency = value; }
	inline bool getTransparency() { return m_hasTransparency; }

	inline void useFakeLighting(bool value) { m_useFakeLighting = value; }
	inline bool isUsingFakeLighting() { return m_useFakeLighting; }

	void setSpecularMap(int value) { 
		m_specularMap = value;
		m_hasSpecularMap = true; 
	}
	inline bool hasSpecularMap() { return m_hasSpecularMap; }
	inline unsigned int getSpecularMap() { return m_specularMap; }

	inline void setNumberOfRows(int value) { m_numberOfRows = value; }
	inline bool getNumberOfRows() { return m_numberOfRows; }
private:
	unsigned int m_textureID,
				 m_normalMap,
				 m_specularMap;

	float m_shineDamper = 10;
	float m_reflectivity = 0.5f;

	bool m_hasTransparency = false,
		 m_useFakeLighting = false,
		 m_hasSpecularMap = false;

	int m_numberOfRows = 1;
};