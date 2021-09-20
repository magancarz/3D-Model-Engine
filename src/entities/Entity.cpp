#include "Entity.h"

Entity::Entity(TexturedModel model, glm::vec3 position, float rx, float ry, float rz, float scale)
	: m_model(model), m_position(position), m_rx(rx), m_ry(ry), m_rz(rz), m_scale(scale) {

}

void Entity::increasePosition(float dx, float dy, float dz) {
	m_position.x += dx;
	m_position.y += dy;
	m_position.z += dz;
}

void Entity::increaseRotation(float rx, float ry, float rz) {
	m_rx += rx;
	m_ry += ry;
	m_rz += rz;
}