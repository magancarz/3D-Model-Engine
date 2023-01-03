#include "FBO.h"

#include <GL/glew.h>

#include "toolbox/DisplayManager.h"

FBO::FBO(unsigned int width, unsigned int height, unsigned int depthBufferType) :
m_width(width), m_height(height) {
	initialiseFrameBuffer(depthBufferType);
}

FBO::FBO(unsigned int width, unsigned int height) :
m_width(width), m_height(height), m_multisampleAndMultiTarget(true) {
	initialiseFrameBuffer(FBO_DEPTH_RENDER_BUFFER);
}

void FBO::cleanUp() {
	glDeleteFramebuffers(1, &m_frameBuffer);
	glDeleteTextures(1, &m_colorTexture);
	glDeleteTextures(1, &m_depthTexture);
	glDeleteRenderbuffers(1, &m_depthBuffer);
	glDeleteRenderbuffers(1, &m_colorBuffer1);
	glDeleteRenderbuffers(1, &m_colorBuffer2);
}

void FBO::bindFrameBuffer() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
	glViewport(0, 0, m_width, m_height);
}

void FBO::unbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT);
}

void FBO::bindToRead() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void FBO::resolveToFBO(unsigned int readBuffer, FBO* outputFBO) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, outputFBO->m_frameBuffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
	glReadBuffer(readBuffer);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, outputFBO->m_width, outputFBO->m_height,
					  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	unbindFrameBuffer();
}

void FBO::resolveToScreen() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT,
					  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	unbindFrameBuffer();
}

void FBO::initialiseFrameBuffer(unsigned int type) {
	createFrameBuffer();
	if(m_multisampleAndMultiTarget) {
		m_colorBuffer1 = createMultisampleColorAttachment(GL_COLOR_ATTACHMENT0);
		m_colorBuffer2 = createMultisampleColorAttachment(GL_COLOR_ATTACHMENT1);
	} else {
		createTextureAttachment();
	}

	if(type == FBO_DEPTH_RENDER_BUFFER) {
		createDepthBufferAttachment();
	} else if(type == FBO_DEPTH_TEXTURE) {
		createDepthTextureAttachment();
	}

	unbindFrameBuffer();
}

void FBO::createFrameBuffer() {
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	determineDrawBuffers();
}

void FBO::determineDrawBuffers() {
	unsigned int drawBuffers[2];
	drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	if(m_multisampleAndMultiTarget) {
		drawBuffers[1] = GL_COLOR_ATTACHMENT1;
	}
	glDrawBuffers(m_multisampleAndMultiTarget? 2 : 1, drawBuffers);
}

void FBO::createTextureAttachment() {
	glGenTextures(1, &m_colorTexture);
	glBindTexture(GL_TEXTURE_2D, m_colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);
}

void FBO::createDepthTextureAttachment() {
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT,
				 GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
}

int FBO::createMultisampleColorAttachment(int attachment) {
	unsigned int colorBuffer = 0;
	glGenRenderbuffers(1, &colorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, colorBuffer);

	return colorBuffer;
}

void FBO::createDepthBufferAttachment() {
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	if(!m_multisampleAndMultiTarget) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
	} else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, m_width, m_height);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
}