#include "Entity.h"

Entity::Entity(
	std::shared_ptr<TexturedModel> model,
	const glm::vec3& position,
	const float rx, const float ry, const float rz, const float scale) :
m_model(std::move(model)), m_position(position), m_rx(rx), m_ry(ry), m_rz(rz), m_scale(scale) {

}

Entity::Entity(
	std::shared_ptr<TexturedModel> model,
	const int texture_index,
	const glm::vec3& position,
	const float rx, const float ry, const float rz, const float scale) :
m_model(std::move(model)), m_position(position), m_rx(rx), m_ry(ry), m_rz(rz), m_scale(scale), m_texture_index(texture_index) {

}

float Entity::get_texture_x_offset() const {
	const int column = m_texture_index % m_model->getTexture().getNumberOfRows();
	return static_cast<float>(column) / static_cast<float>(m_model->getTexture().getNumberOfRows());
}

float Entity::get_texture_y_offset() const {
	const int row = m_texture_index / m_model->getTexture().getNumberOfRows();
	return static_cast<float>(row) / static_cast<float>(m_model->getTexture().getNumberOfRows());
}

std::shared_ptr<TexturedModel> Entity::get_textured_model() { return m_model; }
glm::vec3 Entity::get_position() const { return m_position; }
float Entity::get_rot_x() const { return m_rx; }
float Entity::get_rot_y() const { return m_ry; }
float Entity::get_rot_z() const { return m_rz; }
float Entity::get_scale() const { return m_scale; }

void Entity::set_textured_model(const std::shared_ptr<TexturedModel>& model) { m_model = model; }
void Entity::set_position(const glm::vec3& position) { m_position = position; }
void Entity::set_rot_x(const float rx) { m_rx = rx; }
void Entity::set_rot_y(const float ry) { m_ry = ry; }
void Entity::set_rot_z(const float rz) { m_rz = rz; }
void Entity::set_scale(const float scale) { m_scale = scale; }

void Entity::increase_position(const float dx, const float dy, const float dz) {
	m_position.x += dx;
	m_position.y += dy;
	m_position.z += dz;
}

void Entity::increase_rotation(const float rx, const float ry, const float rz) {
	m_rx += rx;
	m_ry += ry;
	m_rz += rz;
}