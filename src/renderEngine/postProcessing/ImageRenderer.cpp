#include "ImageRenderer.h"

#include <gl/glew.h>

#include "toolbox/DisplayManager.h"

ImageRenderer::ImageRenderer() {

}

ImageRenderer::ImageRenderer(const unsigned int width, const unsigned int height) {
	m_fbo = std::make_shared<FBO>(width, height, FBO_NONE);
}

void ImageRenderer::render_quad() const {
	if(m_fbo != nullptr) {
		m_fbo->bind_frame_buffer();
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if(m_fbo != nullptr) {
		m_fbo->unbind_frame_buffer();
	}
}