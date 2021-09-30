#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer() {

}

TerrainRenderer::TerrainRenderer(TerrainShader* shader, glm::mat4 projectionMatrix)
	: m_shader(shader) {
	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
}

void TerrainRenderer::render(std::vector<Terrain*>* terrains) {
	for(std::vector<Terrain*>::iterator it = terrains->begin(); it < terrains->end(); it++) {
		prepareTerrain(*it);
		loadModelMatrix(*it);
		glDrawElements(GL_TRIANGLES, (*it)->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
		unbindTexturedModel();
	}
}

void TerrainRenderer::prepareTerrain(Terrain* terrain) {
	RawModel* model = terrain->getModel();
	glBindVertexArray(model->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	ModelTexture* texture = terrain->getTexture();
	m_shader->loadShineVariables(texture->getShineDamper(), texture->getReflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getID());
}

void TerrainRenderer::unbindTexturedModel() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void TerrainRenderer::loadModelMatrix(Terrain* terrain) {
	glm::mat4 transformationMatrix = createTransformationMatrix(glm::vec3(terrain->getX(), 0, terrain->getZ()), 0, 0, 0, 1);
	m_shader->loadTransformationMatrix(transformationMatrix);
	ModelTexture* texture = terrain->getTexture();
	m_shader->loadShineVariables(texture->getShineDamper(), texture->getReflectivity());
}