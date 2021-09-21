#pragma once

#include "../Headers.h"

#include "../models/TexturedModel.h"

class Entity {
public:
	Entity(TexturedModel model, glm::vec3 position, float rx, float ry, float rz, float scale);

	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float rx, float ry, float rz);

	inline TexturedModel getTexturedModel() const { return m_model; };
	inline glm::vec3 getPosition() const { return m_position; };
	inline float getRotX() const { return m_rx; };
	inline float getRotY() const { return m_ry; };
	inline float getRotZ() const { return m_rz; };
	inline float getScale() const { return m_scale; };

	void setTexturedModel(TexturedModel model) { m_model = model; };
	void setPosition(glm::vec3 position) { m_position = position; };
	void setRotX(float rx) { m_rx = rx; };
	void setRotY(float ry) { m_ry = ry; };
	void setRotZ(float rz) { m_rz = rz; };
	void setScale(float scale) { m_scale = scale; };

private:
	TexturedModel m_model;
	glm::vec3 m_position;
	float m_rx, m_ry, m_rz;
	float m_scale;
};