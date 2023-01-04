#include "ContrastChanger.h"

#include <GL/glew.h>

ContrastChanger::ContrastChanger() {
	m_shader = std::make_unique<ContrastShader>();
	m_renderer = std::make_unique<ImageRenderer>();
}

void ContrastChanger::render(const unsigned int texture) const {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->render_quad();
	m_shader->stop();
}