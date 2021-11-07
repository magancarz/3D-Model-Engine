#include "NormalMappingRenderer.h"

#include "../renderEngine/MasterRenderer.h"

NormalMappingRenderer::NormalMappingRenderer(glm::mat4 proj) {
	m_shader = new NormalMappingShader();
	m_shader->start();
	m_shader->loadProjectionMatrix(proj);
	m_shader->connectTextureUnits();
	m_shader->stop();
}

NormalMappingRenderer::~NormalMappingRenderer() {
	m_shader->cleanUp();
	delete m_shader;
}

void NormalMappingRenderer::render(
	std::map<TexturedModel*, std::vector<Entity*>*>* entities,
	glm::vec4 clipPlane,
	std::vector<Light*>& lights,
	Camera& camera)
{
	m_shader->start();
	prepare(clipPlane, lights, camera);

	for (std::map<TexturedModel*, std::vector<Entity*>*>::iterator it = entities->begin();
	     it != entities->end();
	     it++) {
		TexturedModel* model = (*it).first;

		prepareTexturedModel(model);

		it = entities->find(model);
  		if (it != entities->end()) {
  			std::vector<Entity*>* batch = it->second;

  			for (std::vector<Entity*>::iterator vit = batch->begin();
			     vit != batch->end();
			     vit++) {
  				Entity *entity = *vit;
  				prepareInstance(entity);
  				glDrawElements(GL_TRIANGLES, model->getRawModel().getVertexCount(), GL_UNSIGNED_INT, 0);
  			}
  		}

  		unbindTexturedModel();
	}

	m_shader->stop();
}


void NormalMappingRenderer::prepareTexturedModel(TexturedModel* model)
{
	RawModel& rawModel = model->getRawModel();
	glBindVertexArray(rawModel.getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	ModelTexture& texture = model->getTexture();
	m_shader->loadNumberOfRows(texture.getNumberOfRows());
	if (texture.getTransparency()) {
		disableCulling();
	}
	//m_shader->loadFakeLightingVariable(texture.getUseFakeLighting());
	m_shader->loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->getTexture().getID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, model->getTexture().getNormalMap());
}

void NormalMappingRenderer::unbindTexturedModel()
{
	enableCulling();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}

void NormalMappingRenderer::prepareInstance(Entity* entity)
{
	glm::mat4 transformationMatrix = createTransformationMatrix(
		entity->getPosition(),
		entity->getRotX(), entity->getRotY(), entity->getRotZ(),
		entity->getScale());

	//glm::vec3 pos = entity.getPosition();
	//cout << "pos = " << pos[0] << ", " << pos[1] << ", " << pos[2] << endl;
	//glm::mat4 t = glm::translate(glm::mat4(1.0f), pos);
	//Maths::printMatrix(t, "t");
	//Maths::printMatrix(transformationMatrix, "T");

	m_shader->loadTransformationMatrix(transformationMatrix);
	m_shader->loadOffset(entity->getTextureXOffset(), entity->getTextureYOffset());
}

void NormalMappingRenderer::prepare(
	glm::vec4 clipPlane,
	std::vector<Light*>&lights,
	Camera& camera)
{
	m_shader->loadClipPlane(clipPlane);
	m_shader->loadSkyColor(RED, GREEN, BLUE);
	//m_shader->loadFogVariables(FOG_DENSITY, masterRenderer.FOG_GRADIENT);
	m_shader->loadLights(lights, camera.getView());
	m_shader->loadViewMatrix(camera);
}