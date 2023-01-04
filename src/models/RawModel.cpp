#include "RawModel.h"

RawModel::RawModel(const unsigned int vao_id, const unsigned int vertex_count) :
m_vao_id(vao_id), m_vertex_count(vertex_count) {}

unsigned int RawModel::get_vao_id() const {
	return m_vao_id;
}

unsigned int RawModel::get_vertex_count() const {
	return m_vertex_count;
}