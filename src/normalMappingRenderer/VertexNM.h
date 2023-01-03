#pragma once

#include <memory>
#include <glm/glm.hpp>

#include <vector>

class VertexNM {
public:
	VertexNM(int index, const glm::vec3& position);

	bool is_set() const;

	bool has_same_texture_and_normal(int texture_index_other, int normal_index_other) const;

	int get_index() const;

	glm::vec3 get_position() const;

	int get_texture_index() const;
	void set_texture_index(int texture_index);

	int get_normal_index() const;
	void set_normal_index(int normal_index);

	int get_length() const;

	std::shared_ptr<VertexNM> get_duplicate_vertex();
	void set_duplicate_vertex(std::shared_ptr<VertexNM> duplicate_vertex);

	std::shared_ptr<VertexNM> duplicate(int new_index) const;

	void average_tangents();
	void add_tangent(const glm::vec3& tangent);
	glm::vec3 get_average_tangent() const;

private:
	static constexpr int NO_INDEX = -1;

	int m_index;

	glm::vec3 m_position;

	int m_texture_index = NO_INDEX;
	int m_normal_index = NO_INDEX;

	float m_length;

	std::shared_ptr<VertexNM> m_duplicate_vertex = nullptr;

	std::vector<glm::vec3> m_tangents;
	glm::vec3 m_averaged_tangent;
};