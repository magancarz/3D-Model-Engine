#include "GuiRenderer.h"

GuiRenderer::GuiRenderer(Loader* loader) {
	float values[8] = {
		-1,  1,
		-1, -1,
		 1,  1,
		 1, -1
	};
	std::vector<float> positions;
	positions.resize(8);
	for(int i = 0; i < 8; i++) {
		positions[i] = values[i];
	}
	quad = loader->loadToVAO(positions, 2);

	//Create shader
	m_shader = new GuiShader();
}

GuiRenderer::~GuiRenderer() {
	//Clean up GUI shader
	m_shader->cleanUp();
	delete m_shader;
}

void GuiRenderer::render(std::vector<GuiTexture>* guis) {
	m_shader->start();
	glBindVertexArray(quad->getVaoID());
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	//render
	for(std::vector<GuiTexture>::iterator it = guis->begin(); it != guis->end(); it++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, it->getTexture());
		glm::mat4 matrix = createTransformationMatrix(it->getPosition(), it->getScale());
		m_shader->loadTransformation(matrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_shader->stop();
}