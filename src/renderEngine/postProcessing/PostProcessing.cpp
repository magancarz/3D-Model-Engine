#include "PostProcessing.h"

#include <GL/glew.h>

#include "toolbox/DisplayManager.h"

void PostProcessing::post_processing_init(const std::shared_ptr<Loader>& loader) {
	post_processing_quad = loader->loadToVAO(post_processing_positions, 2);
	post_processing_contrast_changer = std::make_unique<ContrastChanger>();
	post_processing_horizontal_blur = std::make_unique<HorizontalBlur>(DisplayManager::WINDOW_WIDTH / 2, DisplayManager::WINDOW_HEIGHT / 2);
	post_processing_vertical_blur = std::make_unique<VerticalBlur>(DisplayManager::WINDOW_HEIGHT / 2);
	post_processing_bright_filter = std::make_unique<BrightFilter>(DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT);
	post_processing_combine_filter = std::make_unique<CombineFilter>();
}

void PostProcessing::post_processing_draw(const unsigned int color_texture, const unsigned int bright_texture) {
	post_processing_start();

	post_processing_horizontal_blur->render(bright_texture);
	post_processing_vertical_blur->render(post_processing_horizontal_blur->get_output_texture());
	post_processing_combine_filter->render(color_texture, post_processing_vertical_blur->get_output_texture());
	
	post_processing_end();
}

void PostProcessing::post_processing_start() {
	glBindVertexArray(post_processing_quad->getVaoID());
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

void PostProcessing::post_processing_end() {
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}