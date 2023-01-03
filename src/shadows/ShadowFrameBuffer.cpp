#include "ShadowFramebuffer.h"

#include <GL/glew.h>

#include "toolbox/DisplayManager.h"

ShadowFrameBuffer::ShadowFrameBuffer(const int width, const int height) :
m_width(width), m_height(height) {

	initialise_frame_buffer();
}

ShadowFrameBuffer::~ShadowFrameBuffer() {
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteTextures(1, &m_shadow_map);
}

void ShadowFrameBuffer::bind_frame_buffer() const {
	bind_frame_buffer(m_fbo, m_width, m_height);
}

void ShadowFrameBuffer::unbind_frame_buffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT);
}

unsigned int ShadowFrameBuffer::get_shadow_map() const { return m_shadow_map; }

void ShadowFrameBuffer::initialise_frame_buffer() {
	m_fbo = create_frame_buffer();
	m_shadow_map = create_depth_buffer_attachment(m_width, m_height);
	unbind_frame_buffer();
}

void ShadowFrameBuffer::bind_frame_buffer(const unsigned int frame_buffer, const int width, const int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
	glViewport(0, 0, width, height);
}

unsigned int ShadowFrameBuffer::create_frame_buffer() {
	unsigned int frame_buffer = 0;
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frame_buffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	return frame_buffer;
}

unsigned int ShadowFrameBuffer::create_depth_buffer_attachment(const int width, const int height) {
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

	return texture;
}