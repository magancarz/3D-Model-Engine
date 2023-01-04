#include "EntityRenderer.h"

#include <gl/glew.h>

#include "../renderEngine/MasterRenderer.h"
#include "../toolbox/Maths.h"

EntityRenderer::EntityRenderer(const glm::mat4& projection_matrix) {
	m_static_shader = std::make_unique<StaticShader>();
	m_static_shader->start();
	m_static_shader->load_projection_matrix(projection_matrix);
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
			glDrawElements(GL_TRIANGLES, static_cast<int>(textured_model->get_raw_model()->get_vertex_count()), GL_UNSIGNED_INT, nullptr);
		}

		unbind_textured_model();
	}
	m_static_shader->stop();
}

void EntityRenderer::prepare_textured_model(const std::shared_ptr<TexturedModel>& textured_model) const {
	const auto& model = textured_model->get_raw_model();
	glBindVertexArray(model->get_vao_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	const auto texture = textured_model->get_texture();
	m_static_shader->load_number_of_rows(static_cast<float>(texture->get_number_of_rows()));
	m_static_shader->load_fake_lighting_variable(texture->is_using_fake_lighting());
	m_static_shader->load_shine_variables(texture->get_shine_damper(), texture->get_reflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->get_id());
	m_static_shader->load_use_specular_map(texture->has_specular_map());
	if(texture->has_specular_map()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture->get_specular_map());
	}
}

void EntityRenderer::unbind_textured_model() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::prepare_instance(const std::shared_ptr<Entity>& entity) const {
	const auto transformation_matrix = create_transformation_matrix(
		entity->get_position(),
		entity->get_rot_x(),
		entity->get_rot_y(),
		entity->get_rot_z(),
		entity->get_scale());

	m_static_shader->load_transformation_matrix(transformation_matrix);
	m_static_shader->load_offset(glm::vec2(entity->get_texture_x_offset(), entity->get_texture_y_offset()));
	const auto texture = entity->get_textured_model()->get_texture();
	m_static_shader->load_shine_variables(texture->get_shine_damper(), texture->get_reflectivity());
}