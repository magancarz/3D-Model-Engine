#pragma once

class RawModel {
public:
	RawModel(unsigned int vao_id, unsigned int vertex_count);

	unsigned int get_vao_id() const;
	unsigned int get_vertex_count() const;

private:
	unsigned int m_vao_id,
				 m_vertex_count;
};