#include "VertexNM.h"

VertexNM::VertexNM(const int index, const glm::vec3& position) :
m_index(index), m_position(position), m_length(position.length()) {}

bool VertexNM::is_set() const {
	return m_texture_index != NO_INDEX && m_normal_index != NO_INDEX;
}

bool VertexNM::has_same_texture_and_normal(const int texture_index_other, const int normal_index_other) const {
	return texture_index_other == m_texture_index && normal_index_other == m_normal_index;
}

int VertexNM::get_index() const {
	return m_index;
}

glm::vec3 VertexNM::get_position() const {
	return m_position;
}

int VertexNM::get_texture_index() const {
	return m_texture_index;
}

void VertexNM::set_texture_index(const int texture_index) {
	m_texture_index = texture_index;
}

int VertexNM::get_normal_index() const {
	return m_normal_index;
}

void VertexNM::set_normal_index(const int normal_index) {
	m_normal_index = normal_index;
}

int VertexNM::get_length() const {
	return m_length;
}

std::shared_ptr<VertexNM> VertexNM::get_duplicate_vertex() {
	return m_duplicate_vertex;
}

void VertexNM::set_duplicate_vertex(std::shared_ptr<VertexNM> duplicate_vertex) {
	m_duplicate_vertex = std::move(duplicate_vertex);
}

std::shared_ptr<VertexNM> VertexNM::duplicate(const int new_index) const {
	auto vertex = std::make_shared<VertexNM>(new_index, m_position);
	vertex->m_tangents = m_tangents;
	return vertex;
}

void VertexNM::average_tangents() {
	if(m_tangents.empty())
		return;

	for(const auto& tangent : m_tangents) {
		m_averaged_tangent += tangent;
	}
	m_averaged_tangent = glm::normalize(m_averaged_tangent);
}

void VertexNM::add_tangent(const glm::vec3& tangent) {
	m_tangents.push_back(tangent);
}

glm::vec3 VertexNM::get_average_tangent() const {
	return m_averaged_tangent;
}