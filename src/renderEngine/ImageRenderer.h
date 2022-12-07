#pragma once

#include "FBO.h"

class ImageRenderer {
public:
	ImageRenderer(unsigned int width, unsigned int height);
	
	ImageRenderer();

	void renderQuad();

	inline unsigned int getOutputTexture() { return m_fbo->getColorTexture(); }

	void cleanUp();

private:
	FBO* m_fbo;
};