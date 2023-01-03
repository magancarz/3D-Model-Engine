#pragma once

#include "CombineShader.h"
#include "renderEngine/postProcessing/ImageRenderer.h"

class CombineFilter {
public:
	CombineFilter();

	void render(unsigned int color_texture, unsigned int highlight_texture) const;

private:
	std::unique_ptr<CombineShader> m_shader;
	std::unique_ptr<ImageRenderer> m_renderer;
};
