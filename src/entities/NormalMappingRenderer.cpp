#include "NormalMappingRenderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../renderEngine/MasterRenderer.h"
#include "../toolbox/Maths.h"

NormalMappingRenderer::NormalMappingRenderer(const glm::mat4& projection_matrix) {
	m_normal_mapping_shader = std::make_unique<NormalMappingShader>();
	m_normal_mapping_shader->start();
	m_normal_mapping_shader->load_projection_matrix(projection_matrix);
	m_normal_mapping_shader->connect_texture_units();
	m_normal_mapping_shader->stop();
}

void NormalMappingRenderer::render(
		const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entity_map,
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const {

	m_normal_mapping_shader->start();
	prepare(lights, camera, clip_plane);

	for(const auto& [textured_model, entities] : entity_map) {
		prepare_textured_model(textured_model);

		const auto textured_model_vertex_count = textured_model->get_raw_model()->get_vertex_count();
  		for(const auto& entity : entities) {
  			prepare_instance(entity);
  			glDrawElements(GL_TRIANGLES, textured_model_vertex_count, GL_UNSIGNED_INT, nullptr);
  		}

  		unbind_textured_model();
	}

	m_normal_mapping_shader->stop();
}


void NormalMappingRenderer::prepare_textured_model(const std::shared_ptr<TexturedModel>& textured_model) const {
	const auto raw_model = textured_model->get_raw_model();
	glBindVertexArray(raw_model->get_vao_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	const auto texture = textured_model->get_texture();
	m_normal_mapping_shader->load_number_of_rows(texture->get_number_of_rows());
	m_normal_mapping_shader->load_fake_lighting_variable(texture->is_using_fake_lighting());
	m_normal_mapping_shader->load_shine_variables(texture->get_shine_damper(), texture->get_reflectivity());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->get_id());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture->get_normal_map());
}

void NormalMappingRenderer::unbind_textured_model() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}

void NormalMappingRenderer::prepare_instance(const std::shared_ptr<Entity>& entity) const {
	const auto transformation_matrix = create_transformation_matrix(
		entity->get_position(),
		entity->get_rot_x(), entity->get_rot_y(), entity->get_rot_z(),
		entity->get_scale());

	m_normal_mapping_shader->load_transformation_matrix(transformation_matrix);
	m_normal_mapping_shader->load_offset(entity->get_texture_x_offset(), entity->get_texture_y_offset());
}

void NormalMappingRenderer::prepare(
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const {

	m_normal_mapping_shader->load_clip_plane(clip_plane);
	m_normal_mapping_shader->load_sky_color(MasterRenderer::RED, MasterRenderer::GREEN, MasterRenderer::BLUE);
	//m_static_shader->load_fog_variables(FOG_DENSITY, masterRenderer.FOG_GRADIENT);
	m_normal_mapping_shader->load_lights(lights, camera->get_view());
	m_normal_mapping_shader->load_view_matrix(camera);
}