#include "Renderer.h"

void Renderer::prepare() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0, 0, 0, 1);
}

void Renderer::render(std::map<TexturedModel*, std::vector<Entity*>*>* entities) {
	for(std::map<TexturedModel*, std::vector<Entity*>*>::iterator it = entities->begin(); it != entities->end(); it++) {
		TexturedModel* texturedModel = (*it).first;
		prepareTexturedModel(*texturedModel);
		it = entities->find(texturedModel);
		if(it != entities->end()) {
			std::vector<Entity*>* batch = it->second;

			for(std::vector<Entity*>::iterator sit = batch->begin(); sit != batch->end(); sit++) {
				Entity* entity = *sit;
				prepareInstance(*entity);
				glDrawElements(GL_TRIANGLES, texturedModel->getRawModel().getVertexCount(), GL_UNSIGNED_INT, 0);
			}

			unbindTexturedModel();
		}
	}
}

void Renderer::prepareTexturedModel(TexturedModel& texturedModel) {
	RawModel model = texturedModel.getRawModel();
	glBindVertexArray(model.getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	ModelTexture texture = texturedModel.getTexture();
	m_shader.loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getID());
}

void Renderer::unbindTexturedModel() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void Renderer::prepareInstance(Entity& entity) {
	glm::mat4 transformationMatrix = createTransformationMatrix(entity.getPosition(), entity.getRotX(), entity.getRotY(), entity.getRotZ(), entity.getScale());
	m_shader.loadTransformationMatrix(transformationMatrix);
	ModelTexture texture = entity.getTexturedModel().getTexture();
	m_shader.loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
}