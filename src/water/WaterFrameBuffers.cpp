#include "WaterFrameBuffers.h"

#include <GL/glew.h>

WaterFrameBuffers::WaterFrameBuffers() {
	initialise_reflection_frame_buffer();
	initialise_refraction_frame_buffer();
}

WaterFrameBuffers::~WaterFrameBuffers() {
	glDeleteFramebuffers(1, &m_reflection_frame_buffer);
	glDeleteTextures(1, &m_reflection_texture);
	glDeleteRenderbuffers(1, &m_reflection_depth_buffer);
	glDeleteFramebuffers(1, &m_refraction_frame_buffer);
	glDeleteTextures(1, &m_refraction_texture);
	glDeleteRenderbuffers(1, &m_refraction_depth_texture);
}

void WaterFrameBuffers::bind_reflection_frame_buffer() const {
	bind_frame_buffer(m_reflection_frame_buffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffers::bind_refraction_frame_buffer() const {
	bind_frame_buffer(m_refraction_frame_buffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffers::unbind_current_frame_buffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

unsigned int WaterFrameBuffers::get_reflection_texture() const { return m_reflection_texture; }
unsigned int WaterFrameBuffers::get_refraction_texture() const { return m_refraction_texture; }
unsigned int WaterFrameBuffers::get_refraction_depth_texture() const { return m_refraction_depth_texture; }

void WaterFrameBuffers::initialise_reflection_frame_buffer() {
	m_reflection_frame_buffer = create_frame_buffer();
	m_reflection_texture = create_texture_attachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	m_reflection_depth_buffer = create_depth_buffer_attachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbind_current_frame_buffer();
}

void WaterFrameBuffers::initialise_refraction_frame_buffer() {
	m_refraction_frame_buffer = create_frame_buffer();
	m_refraction_texture = create_texture_attachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	m_refraction_depth_texture = create_depth_texture_attachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbind_current_frame_buffer();
}

void WaterFrameBuffers::bind_frame_buffer(const unsigned int frame_buffer, const int width, const int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glViewport(0, 0, width, height);
}

unsigned int WaterFrameBuffers::create_frame_buffer() {
	unsigned int frame_buffer;
	glGenFramebuffers(1, &frame_buffer);
	//generate name for frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	//create the frame buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//indicate that we will always render to color attachment 0
	return frame_buffer;
}

unsigned int WaterFrameBuffers::create_texture_attachment(const int width, const int height) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}

unsigned int WaterFrameBuffers::create_depth_texture_attachment(const int width, const int height) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}

unsigned int WaterFrameBuffers::create_depth_buffer_attachment(const int width, const int height) {
	unsigned int depth_buffer;
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	return depth_buffer;
}