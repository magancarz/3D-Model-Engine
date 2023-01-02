#include "ShadowFramebuffer.h"

#include <GL/glew.h>

#include "toolbox/DisplayManager.h"

ShadowFrameBuffer::ShadowFrameBuffer(int width, int height)
	: WIDTH(width), HEIGHT(height) {
	initialiseFrameBuffer();
}

ShadowFrameBuffer::~ShadowFrameBuffer() {
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteTextures(1, &m_shadowMap);
}

void ShadowFrameBuffer::bindFrameBuffer() {
	bindFrameBuffer(m_fbo, WIDTH, HEIGHT);
}

void ShadowFrameBuffer::unbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void ShadowFrameBuffer::initialiseFrameBuffer() {
	m_fbo = createFrameBuffer();
	m_shadowMap = createDepthBufferAttachment(WIDTH, HEIGHT);
	unbindFrameBuffer();
}

void ShadowFrameBuffer::bindFrameBuffer(unsigned int frameBuffer, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

unsigned int ShadowFrameBuffer::createFrameBuffer() {
	unsigned int frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frameBuffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	return frameBuffer;
}

unsigned int ShadowFrameBuffer::createDepthBufferAttachment(int width, int height) {
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