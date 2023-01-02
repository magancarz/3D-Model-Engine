#include "EntityRenderer.h"

#include <gl/glew.h>

#include "MasterRenderer.h"
#include "toolbox/Maths.h"

EntityRenderer::EntityRenderer(std::unique_ptr<StaticShader> static_shader, const glm::mat4& projection_matrix) :
m_static_shader(std::move(static_shader)), m_projection_matrix(projection_matrix) {

	m_static_shader->start();
	m_static_shader->load_projection_matrix(m_projection_matrix);
	m_static_shader->connect_texture_units();
	m_static_shader->stop();
}

void EntityRenderer::render(
		const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entity_map,
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const {

	m_static_shader->start();
	m_static_shader->load_clip_plane(clip_plane);
	m_static_shader->load_sky_color(MasterRenderer::RED, MasterRenderer::GREEN, MasterRenderer::BLUE);
	m_static_shader->load_lights(lights);
	m_static_shader->load_view_matrix(camera);

	for(const auto& [textured_model, entities] : entity_map) {
		prepare_textured_model(textured_model);

		for(const auto& entity : entities) {
			prepare_instance(entity);
			glDrawElements(GL_TRIANGLES, textured_model->getRawModel().getVertexCount(), GL_UNSIGNED_INT, nullptr);
		}

		unbind_textured_model();
	}
	m_static_shader->stop();
}

void EntityRenderer::prepare_textured_model(const std::shared_ptr<TexturedModel>& textured_model) const {
	const auto& model = textured_model->getRawModel();
	glBindVertexArray(model.getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	auto texture = textured_model->getTexture();
	m_static_shader->load_number_of_rows(texture.getNumberOfRows());
	m_static_shader->load_fake_lighting_variable(texture.isUsingFakeLighting());
	m_static_shader->load_shine_variables(texture.getShineDamper(), texture.getReflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getID());
	m_static_shader->load_use_specular_map(texture.hasSpecularMap());
	if(texture.hasSpecularMap()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture.getSpecularMap());
	}
}

void EntityRenderer::unbind_textured_model() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::prepare_instance(const std::shared_ptr<Entity>& entity) const {
	const auto transformation_matrix = createTransformationMatrix(
		entity->get_position(),
		entity->get_rot_x(),
		entity->get_rot_y(),
		entity->get_rot_z(),
		entity->get_scale());
	m_static_shader->load_transformation_matrix(transformation_matrix);
	m_static_shader->load_offset(glm::vec2(entity->get_texture_x_offset(), entity->get_texture_y_offset()));
	auto texture = entity->get_textured_model()->getTexture();
	m_static_shader->load_shine_variables(texture.getShineDamper(), texture.getReflectivity());
}