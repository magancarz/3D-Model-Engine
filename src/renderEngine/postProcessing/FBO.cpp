#include "FBO.h"

#include <GL/glew.h>

#include "toolbox/DisplayManager.h"

FBO::FBO(const int width, const int height, const unsigned int depth_buffer_type) :
m_width(width), m_height(height) {
	initialise_frame_buffer(depth_buffer_type);
}

FBO::FBO(const int width, const int height) :
m_width(width), m_height(height), m_multi_sample_and_multi_target(true) {
	initialise_frame_buffer(FBO_DEPTH_RENDER_BUFFER);
}

void FBO::clean_up() const {
	glDeleteFramebuffers(1, &m_frame_buffer);
	glDeleteTextures(1, &m_color_texture);
	glDeleteTextures(1, &m_depth_texture);
	glDeleteRenderbuffers(1, &m_depth_buffer);
	glDeleteRenderbuffers(1, &m_color_buffer1);
	glDeleteRenderbuffers(1, &m_color_buffer2);
}

void FBO::bind_frame_buffer() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frame_buffer);
	glViewport(0, 0, m_width, m_height);
}

void FBO::unbind_frame_buffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT);
}

void FBO::bind_to_read() const {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frame_buffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void FBO::resolve_to_fbo(const unsigned int read_buffer, const std::unique_ptr<FBO>& output_fbo) const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output_fbo->m_frame_buffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frame_buffer);
	glReadBuffer(read_buffer);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, output_fbo->m_width, output_fbo->m_height,
					  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	unbind_frame_buffer();
}

void FBO::resolve_to_screen() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frame_buffer);
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT,
					  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	unbind_frame_buffer();
}

void FBO::initialise_frame_buffer(const unsigned int type) {
	create_frame_buffer();
	if(m_multi_sample_and_multi_target) {
		m_color_buffer1 = create_multi_sample_color_attachment(GL_COLOR_ATTACHMENT0);
		m_color_buffer2 = create_multi_sample_color_attachment(GL_COLOR_ATTACHMENT1);
	} else {
		create_texture_attachment();
	}

	if(type == FBO_DEPTH_RENDER_BUFFER) {
		create_depth_buffer_attachment();
	} else if(type == FBO_DEPTH_TEXTURE) {
		create_depth_texture_attachment();
	}

	unbind_frame_buffer();
}

void FBO::create_frame_buffer() {
	glGenFramebuffers(1, &m_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
	determine_draw_buffers();
}

void FBO::determine_draw_buffers() const {
	unsigned int draw_buffers[2];
	draw_buffers[0] = GL_COLOR_ATTACHMENT0;
	if(m_multi_sample_and_multi_target) {
		draw_buffers[1] = GL_COLOR_ATTACHMENT1;
	}
	glDrawBuffers(m_multi_sample_and_multi_target? 2 : 1, draw_buffers);
}

void FBO::create_texture_attachment() {
	glGenTextures(1, &m_color_texture);
	glBindTexture(GL_TEXTURE_2D, m_color_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture, 0);
}

void FBO::create_depth_texture_attachment() {
	glGenTextures(1, &m_depth_texture);
	glBindTexture(GL_TEXTURE_2D, m_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT,
				 GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);
}

unsigned int FBO::create_multi_sample_color_attachment(const int attachment) const {
	unsigned int color_buffer = 0;
	glGenRenderbuffers(1, &color_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, color_buffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, color_buffer);

	return color_buffer;
}

void FBO::create_depth_buffer_attachment() {
	glGenRenderbuffers(1, &m_depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
	if(!m_multi_sample_and_multi_target) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
	} else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, m_width, m_height);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer);
}

unsigned int FBO::get_color_texture() const { return m_color_texture; }
unsigned int FBO::get_depth_texture() const { return m_depth_texture; }