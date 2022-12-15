#pragma once

#include "Headers.h"
#include "ImageRenderer.h"
#include "../shaders/HorizontalBlurShader.h"

class HorizontalBlur {
public:
	HorizontalBlur(unsigned int targetFBOWidth, unsigned int targetFBOHeight);

	void render(unsigned int texture);

	inline unsigned int getOutputTexture() { return m_renderer->getOutputTexture(); }

	void cleanUp();

private:
	ImageRenderer* m_renderer;
	HorizontalBlurShader* m_shader;
};