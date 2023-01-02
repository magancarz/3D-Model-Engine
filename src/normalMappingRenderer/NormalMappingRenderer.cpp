#include "NormalMappingRenderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../renderEngine/MasterRenderer.h"
#include "toolbox/Maths.h"

NormalMappingRenderer::NormalMappingRenderer(glm::mat4 proj) {
	m_shader = new NormalMappingShader();
	m_shader->start();
	m_shader->loadProjectionMatrix(proj);
	m_shader->connectTextureUnits();
	m_shader->stop();
}

NormalMappingRenderer::~NormalMappingRenderer() {
	m_shader->clean_up();
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
	//m_static_shader->load_fake_lighting_variable(texture.getUseFakeLighting());
	m_shader->loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->getTexture().getID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, model->getTexture().getNormalMap());
}

void NormalMappingRenderer::unbindTexturedModel() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}

void NormalMappingRenderer::prepareInstance(Entity* entity)
{
	glm::mat4 transformationMatrix = createTransformationMatrix(
		entity->get_position(),
		entity->get_rot_x(), entity->get_rot_y(), entity->get_rot_z(),
		entity->get_scale());

	//glm::vec3 pos = entity.get_position();
	//cout << "pos = " << pos[0] << ", " << pos[1] << ", " << pos[2] << endl;
	//glm::mat4 t = glm::translate(glm::mat4(1.0f), pos);
	//Maths::printMatrix(t, "t");
	//Maths::printMatrix(transformationMatrix, "T");

	m_shader->loadTransformationMatrix(transformationMatrix);
	m_shader->loadOffset(entity->get_texture_x_offset(), entity->get_texture_y_offset());
}

void NormalMappingRenderer::prepare(
	glm::vec4 clipPlane,
	std::vector<Light*>&lights,
	Camera& camera)
{
	m_shader->loadClipPlane(clipPlane);
	m_shader->loadSkyColor(MasterRenderer::RED, MasterRenderer::GREEN, MasterRenderer::BLUE);
	//m_static_shader->loadFogVariables(FOG_DENSITY, masterRenderer.FOG_GRADIENT);
	m_shader->loadLights(lights, camera.getView());
	m_shader->loadViewMatrix(camera);
}