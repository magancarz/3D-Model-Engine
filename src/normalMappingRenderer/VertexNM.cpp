#include "VertexNM.h"

VertexNM::VertexNM(int index, glm::vec3 position)
	: m_index(index), m_position(position), m_length(position.length()) {}

VertexNM::~VertexNM() {
	//if(m_duplicateVertex != nullptr)
		//delete m_duplicateVertex;
}

void VertexNM::addTangent(glm::vec3 tangent) {
	m_tangents.push_back(tangent);
}

VertexNM* VertexNM::duplicate(int newIndex) {
	VertexNM* vertex = new VertexNM(newIndex, m_position);
	vertex->m_tangents = m_tangents;
	return vertex;
}

void VertexNM::averageTangents() {
	if(m_tangents.empty()) return;

	for(std::vector<glm::vec3>::iterator it = m_tangents.begin(); it != m_tangents.end(); it++) {
		m_averagedTangent += *it;
	}
	m_averagedTangent = glm::normalize(m_averagedTangent);
}