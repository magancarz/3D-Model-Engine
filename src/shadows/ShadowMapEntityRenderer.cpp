#include "ShadowMapEntityRenderer.h"

#include <GL/glew.h>

#include "toolbox/Maths.h"

void ShadowMapEntityRenderer::render(std::map<TexturedModel*, std::vector<Entity*>*>* entitiesMap) {
	for(std::map<TexturedModel*, std::vector<Entity*>*>::iterator it = entitiesMap->begin(); it != entitiesMap->end(); it++) {
		TexturedModel* model = it->first;
		RawModel* rawModel = &(model->getRawModel());
		bindModel(rawModel);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->getTexture().getID());
		std::vector<Entity*>* entities = it->second;
		for(std::vector<Entity*>::iterator sit = entities->begin(); sit != entities->end(); sit++) {
			Entity* entity = *sit;
			prepareInstance(entity);
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
	glm::mat4 transformationMatrix = createTransformationMatrix(entity->getPosition(), entity->getRotX(), entity->getRotY(), entity->getRotZ(), entity->getScale());
	glm::mat4 mvpMatrix = (*m_projectionViewMatrix) * transformationMatrix;
	m_shader->loadMVPMatrix(mvpMatrix);
}