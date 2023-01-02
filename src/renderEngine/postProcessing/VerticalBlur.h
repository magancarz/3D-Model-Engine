#pragma once

#include "VerticalBlurShader.h"
#include "renderEngine/ImageRenderer.h"

class VerticalBlur {
public:
	VerticalBlur(unsigned int targetFBOWidth, unsigned int targetFBOHeight);

	void render(unsigned int texture);

	inline unsigned int getOutputTexture() { return m_renderer->getOutputTexture(); }

	void cleanUp();

private:
	ImageRenderer* m_renderer;
	VerticalBlurShader* m_shader;
};
