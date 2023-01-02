#include "EntityRenderer.h"

EntityRenderer::EntityRenderer(StaticShader* shader, glm::mat4 projectionMatrix)
	: m_shader(shader), m_projectionMatrix(projectionMatrix) {
	m_shader->start();
	m_shader->loadProjectionMatrix(m_projectionMatrix);
	m_shader->connectTextureUnits();
	m_shader->stop();
}

void EntityRenderer::render(std::map<TexturedModel*, std::vector<Entity*>*>* entities) {
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
	m_shader->stop();
}

void EntityRenderer::prepareTexturedModel(TexturedModel& texturedModel) {
	RawModel& model = texturedModel.getRawModel();
	glBindVertexArray(model.getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	ModelTexture& texture = texturedModel.getTexture();
	m_shader->loadNumberOfRows(texture.getNumberOfRows());
	m_shader->loadFakeLightingVariable(texture.isUsingFakeLighting());
	m_shader->loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getID());
	m_shader->loadUseSpecularMap(texture.hasSpecularMap());
	if(texture.hasSpecularMap()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture.getSpecularMap());
	}
}

void EntityRenderer::unbindTexturedModel() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(Entity& entity) {
	glm::mat4 transformationMatrix = createTransformationMatrix(entity.getPosition(), entity.getRotX(), entity.getRotY(), entity.getRotZ(), entity.getScale());
	m_shader->loadTransformationMatrix(transformationMatrix);
	m_shader->loadOffset(glm::vec2(entity.getTextureXOffset(), entity.getTextureYOffset()));
	ModelTexture texture = entity.getTexturedModel().getTexture();
	m_shader->loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
}