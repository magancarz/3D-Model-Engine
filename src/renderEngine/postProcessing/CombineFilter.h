#pragma once

#include "CombineShader.h"
#include "renderEngine/ImageRenderer.h"

class CombineFilter {
public:
	CombineFilter();

	~CombineFilter();

	void render(unsigned int colorTexture, unsigned int highlightTexture);

private:
	ImageRenderer* m_renderer;
	CombineShader* m_shader;

};
