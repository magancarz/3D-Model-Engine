#include "ShadowMapEntityRenderer.h"

#include <GL/glew.h>

#include "toolbox/Maths.h"

void ShadowMapEntityRenderer::render(const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entitiesMap) {
	for(const auto& [textured_model, entities] : entitiesMap) {
		RawModel* rawModel = &(textured_model->getRawModel());
		bindModel(rawModel);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textured_model->getTexture().getID());
		for(const auto& entity : entities) {
			prepareInstance(entity.get());
			glDrawElements(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0);
		}
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void ShadowMapEntityRenderer::bindModel(RawModel* rawModel) {
	glBindVertexArray(rawModel->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void ShadowMapEntityRenderer::prepareInstance(Entity* entity) {
	glm::mat4 transformationMatrix = createTransformationMatrix(entity->get_position(), entity->get_rot_x(), entity->get_rot_y(), entity->get_rot_z(), entity->get_scale());
	glm::mat4 mvpMatrix = (*m_projectionViewMatrix) * transformationMatrix;
	m_shader->loadMVPMatrix(mvpMatrix);
}