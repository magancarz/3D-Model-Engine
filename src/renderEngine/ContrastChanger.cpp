#include "ContrastChanger.h"

ContrastChanger::ContrastChanger() :
m_shader(new ContrastShader), m_renderer(new ImageRenderer) {

}

void ContrastChanger::render(unsigned int texture) {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->renderQuad();
	m_shader->stop();
}

void ContrastChanger::cleanUp() {
	m_renderer->cleanUp();
	m_shader->cleanUp();
	delete m_renderer;
	delete m_shader;
}