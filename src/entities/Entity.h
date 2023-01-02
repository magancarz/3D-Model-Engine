#pragma once

#include <glm/glm.hpp>

#include "../models/TexturedModel.h"

const float GRAVITY = -50.0f;

class Entity {
public:
	Entity(TexturedModel& model, glm::vec3 position, float rx, float ry, float rz, float scale);

	Entity(TexturedModel& model, int textureIndex, glm::vec3 position, float rx, float ry, float rz, float scale);
	
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float rx, float ry, float rz);

	float getTextureXOffset();
	float getTextureYOffset();

	inline TexturedModel& getTexturedModel() { return m_model; };
	inline glm::vec3 getPosition() const { return m_position; };
	inline float getRotX() const { return m_rx; };
	inline float getRotY() const { return m_ry; };
	inline float getRotZ() const { return m_rz; };
	inline float getScale() const { return m_scale; };

	void setTexturedModel(TexturedModel& model) { m_model = model; };
	void setPosition(glm::vec3 position) { m_position = position; };
	void setRotX(float rx) { m_rx = rx; };
	void setRotY(float ry) { m_ry = ry; };
	void setRotZ(float rz) { m_rz = rz; };
	void setScale(float scale) { m_scale = scale; };

protected:
	const float TERRAIN_HEIGHT = 0;
	
	bool isInAir = false;

private:
	TexturedModel& m_model;

	glm::vec3 m_position;
	float m_rx, m_ry, m_rz;
	float m_scale;

	int m_textureIndex = 0;
};