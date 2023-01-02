#include "EntityRenderer.h"

#include <gl/glew.h>

#include "toolbox/Maths.h"

EntityRenderer::EntityRenderer(StaticShader* shader, glm::mat4 projectionMatrix)
	: m_shader(shader), m_projectionMatrix(projectionMatrix) {
	m_shader->start();
	m_shader->loadProjectionMatrix(m_projectionMatrix);
	m_shader->connectTextureUnits();
	m_shader->stop();
}

void EntityRenderer::render(const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entity_map) {
	for(const auto& [textured_model, entities] : entity_map) {
		prepareTexturedModel(*textured_model);

		for(const auto& entity : entities) {
			prepareInstance(*entity);
			glDrawElements(GL_TRIANGLES, textured_model->getRawModel().getVertexCount(), GL_UNSIGNED_INT, 0);
		}

		unbindTexturedModel();
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
	glm::mat4 transformationMatrix = createTransformationMatrix(entity.get_position(), entity.get_rot_x(), entity.get_rot_y(), entity.get_rot_z(), entity.get_scale());
	m_shader->loadTransformationMatrix(transformationMatrix);
	m_shader->loadOffset(glm::vec2(entity.get_texture_x_offset(), entity.get_texture_y_offset()));
	ModelTexture texture = entity.get_textured_model()->getTexture();
	m_shader->loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
}