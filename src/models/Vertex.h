#pragma once

#include <glm/glm.hpp>

#include <memory>

class Vertex {
public:
	Vertex(int index, const glm::vec3& position);

	bool is_set() const;

	bool has_same_texture_and_normal(int texture_index_other, int normal_index_other) const;

	int get_index() const;

	glm::vec3 get_position() const;

	int get_texture_index() const;
	void set_texture_index(int texture_index);

	int get_normal_index() const;
	void set_normal_index(int normal_index);

	float get_length() const;

	std::shared_ptr<Vertex> get_duplicate_vertex();
	void set_duplicate_vertex(std::shared_ptr<Vertex> duplicate_vertex);

private:
	inline static const int NO_INDEX = -1;

	int m_index;

	glm::vec3 m_position;

	int m_texture_index = NO_INDEX;
	int m_normal_index  = NO_INDEX;

	float m_length;

	std::shared_ptr<Vertex> m_duplicate_vertex = nullptr;
};