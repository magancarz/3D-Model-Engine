#include "Vertex.h"

Vertex::Vertex(const int index, const glm::vec3& position) :
m_index(index), m_position(position), m_length(position.length()) {}

bool Vertex::is_set() const {
	return m_texture_index != NO_INDEX && m_normal_index != NO_INDEX;
}

bool Vertex::has_same_texture_and_normal(const int texture_index_other, const int normal_index_other) const {
	return texture_index_other == m_texture_index && normal_index_other == m_normal_index;
}

int Vertex::get_index() const {
	return m_index;
}

glm::vec3 Vertex::get_position() const {
	return m_position;
}

int Vertex::get_texture_index() const {
	return m_texture_index;
}

void Vertex::set_texture_index(const int texture_index) {
	m_texture_index = texture_index;
}

int Vertex::get_normal_index() const {
	return m_normal_index;
}

void Vertex::set_normal_index(const int normal_index) {
	m_normal_index = normal_index;
}

float Vertex::get_length() const {
	return m_length;
}

std::shared_ptr<Vertex> Vertex::get_duplicate_vertex() {
	return m_duplicate_vertex;
}

void Vertex::set_duplicate_vertex(std::shared_ptr<Vertex> duplicate_vertex) {
	m_duplicate_vertex = std::move(duplicate_vertex);
}