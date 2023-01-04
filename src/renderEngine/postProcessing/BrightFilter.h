#pragma once

#include "BrightFilterShader.h"
#include "../renderEngine/postProcessing/ImageRenderer.h"

class BrightFilter {
public:
	BrightFilter(unsigned int width, unsigned int height);

	void render(unsigned int texture) const;

	unsigned int get_output_texture() const;

private:
	std::unique_ptr<BrightFilterShader> m_shader;
	std::unique_ptr<ImageRenderer> m_renderer;
};
