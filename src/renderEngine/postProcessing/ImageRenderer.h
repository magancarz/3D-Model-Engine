#pragma once

#include "FBO.h"

class ImageRenderer {
public:
	ImageRenderer();

	ImageRenderer(unsigned int width, unsigned int height);

	void render_quad() const;

	unsigned int get_output_texture() const { return m_fbo->get_color_texture(); }

private:
	std::shared_ptr<FBO> m_fbo;
};
