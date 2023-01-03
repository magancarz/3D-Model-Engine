#pragma once

#include "HorizontalBlurShader.h"
#include "renderEngine/postProcessing/ImageRenderer.h"

class HorizontalBlur {
public:
	HorizontalBlur(unsigned int target_fbo_width, unsigned int target_fbo_height);

	void render(unsigned int texture) const;

	unsigned int get_output_texture() const;

private:
	std::unique_ptr<ImageRenderer> m_renderer;
	std::unique_ptr<HorizontalBlurShader> m_shader;
};
