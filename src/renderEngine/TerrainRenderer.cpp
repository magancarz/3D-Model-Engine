#include "TerrainRenderer.h"

#include "toolbox/Maths.h"

TerrainRenderer::TerrainRenderer(TerrainShader* shader, glm::mat4 projectionMatrix)
	: m_shader(shader) {
	m_shader->start();
	m_shader->loadProjectionMatrix(projectionMatrix);
	m_shader->connectTextureUnits();
	m_shader->stop();
}

void TerrainRenderer::render(std::vector<Terrain*>* terrains, glm::mat4 toShadowMapSpace) {
	m_shader->loadToShadowMapSpaceMatrix(toShadowMapSpace);
	for(std::vector<Terrain*>::iterator it = terrains->begin(); it < terrains->end(); it++) {
		prepareTerrain(*it);
		loadModelMatrix(*it);
		glDrawElements(GL_TRIANGLES, (*it)->get_model()->getVertexCount(), GL_UNSIGNED_INT, 0);
		unbindTexturedModel();
	}
}

void TerrainRenderer::prepareTerrain(Terrain* terrain) {
	RawModel* model = terrain->get_model();
	glBindVertexArray(model->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	bindTextures(terrain);
	m_shader->loadShineVariables(1, 0);
}

void TerrainRenderer::bindTextures(Terrain* terrain) {
	const auto terrain_texture_pack = terrain->get_terrain_texture_pack();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain_texture_pack->get_background_texture()->get_texture_id());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrain_texture_pack->get_r_texture()->get_texture_id());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrain_texture_pack->get_g_texture()->get_texture_id());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrain_texture_pack->get_b_texture()->get_texture_id());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain->get_blend_map()->get_texture_id());
}

void TerrainRenderer::unbindTexturedModel() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void TerrainRenderer::loadModelMatrix(Terrain* terrain) {
	glm::mat4 transformationMatrix = createTransformationMatrix(glm::vec3(terrain->get_x(), 0, terrain->get_z()), 0, 0, 0, 1);
	//m_shader->loadTransformationMatrix(transformationMatrix);
	//ModelTexture* texture = terrain->getTexture();
	//m_shader->loadShineVariables(texture->getShineDamper(), texture->getReflectivity());

	m_shader->loadTransformationMatrix(transformationMatrix);
}