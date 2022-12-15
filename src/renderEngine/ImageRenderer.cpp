#include "Headers.h"
#include "ImageRenderer.h"

ImageRenderer::ImageRenderer(unsigned int width, unsigned int height) {
	m_fbo = new FBO(width, height, FBO_NONE);
}

ImageRenderer::ImageRenderer() {}

void ImageRenderer::renderQuad() {
	if(m_fbo != nullptr) {
		m_fbo->bindFrameBuffer();
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if(m_fbo != nullptr) {
		m_fbo->unbindFrameBuffer();
	}
}

void ImageRenderer::cleanUp() {
	if(m_fbo != nullptr) {
		m_fbo->cleanUp();
	}

	delete m_fbo;
}