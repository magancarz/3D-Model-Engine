#pragma once

#include "../Headers.h"

class GuiTexture {
public:
	GuiTexture(int texture, glm::vec2 position, glm::vec2 scale);

	inline int getTexture() { return m_texture; };
	inline glm::vec2 getPosition() { return m_position; };
	inline glm::vec2 getScale() { return m_scale; };
private:
	int m_texture;

	glm::vec2 m_position;
	glm::vec2 m_scale;
};