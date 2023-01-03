#include "PostProcessing.h"

#include <GL/glew.h>

#include "toolbox/DisplayManager.h"

PostProcessing::PostProcessing(const std::shared_ptr<Loader>& loader) {
	m_quad = loader->load_to_vao(QUAD_POSITIONS, 2);
	m_contrast_changer = std::make_unique<ContrastChanger>();
	m_horizontal_blur = std::make_unique<HorizontalBlur>(DisplayManager::WINDOW_WIDTH / 2, DisplayManager::WINDOW_HEIGHT / 2);
	m_vertical_blur = std::make_unique<VerticalBlur>(DisplayManager::WINDOW_WIDTH / 2, DisplayManager::WINDOW_HEIGHT / 2);
	m_bright_filter = std::make_unique<BrightFilter>(DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT);
	m_combine_filter = std::make_unique<CombineFilter>();
}

void PostProcessing::draw(const unsigned int color_texture, const unsigned int bright_texture) const {
	start();

	m_horizontal_blur->render(bright_texture);
	m_vertical_blur->render(m_horizontal_blur->get_output_texture());
	m_combine_filter->render(color_texture, m_vertical_blur->get_output_texture());

	end();
}

void PostProcessing::start() const {
	glBindVertexArray(m_quad->get_vao_id());
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

void PostProcessing::end() {
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}