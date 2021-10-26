#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer(TerrainShader* shader, glm::mat4 projectionMatrix)
	: m_shader(shader) {
	m_shader->start();
	m_shader->loadProjectionMatrix(projectionMatrix);
	m_shader->connectTextureUnits();
	m_shader->stop();
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
	bindTextures(terrain);
	m_shader->loadShineVariables(1, 0);
}

void TerrainRenderer::bindTextures(Terrain* terrain) {
	TerrainTexturePack* texturePack = terrain->getTerrainTexturePack();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePack->getBackgroundTexture().getTextureID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturePack->getrTexture().getTextureID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texturePack->getgTexture().getTextureID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texturePack->getbTexture().getTextureID());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain->getBlendMap()->getTextureID());
}

void TerrainRenderer::unbindTexturedModel() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void TerrainRenderer::loadModelMatrix(Terrain* terrain) {
	glm::mat4 transformationMatrix = createTransformationMatrix(glm::vec3(terrain->getX(), 0, terrain->getZ()), 0, 0, 0, 1);
	//m_shader->loadTransformationMatrix(transformationMatrix);
	//ModelTexture* texture = terrain->getTexture();
	//m_shader->loadShineVariables(texture->getShineDamper(), texture->getReflectivity());

	m_shader->loadTransformationMatrix(transformationMatrix);
}