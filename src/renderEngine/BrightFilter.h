#pragma once

#include "Headers.h"
#include "ImageRenderer.h"
#include "../shaders/BrightFilterShader.h"

class BrightFilter {
public:
	BrightFilter(unsigned int width, unsigned int height);
	
	~BrightFilter();

	void render(unsigned int texture);

	inline int getOutputTexture() const { return m_renderer->getOutputTexture(); }

private:
	ImageRenderer* m_renderer;
	BrightFilterShader* m_shader;
};