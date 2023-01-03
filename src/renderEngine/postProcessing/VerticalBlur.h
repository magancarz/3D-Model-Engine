#pragma once

#include "VerticalBlurShader.h"
#include "renderEngine/postProcessing/ImageRenderer.h"

class VerticalBlur {
public:
	VerticalBlur(unsigned int target_fbo_height);

	void render(unsigned int texture) const;

	unsigned int get_output_texture() const;

private:
	std::unique_ptr<ImageRenderer> m_renderer;
	std::unique_ptr<VerticalBlurShader> m_shader;
};
