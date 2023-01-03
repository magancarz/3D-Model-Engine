#include "ShadowMapEntityRenderer.h"

#include <GL/glew.h>

#include "toolbox/Maths.h"

ShadowMapEntityRenderer::ShadowMapEntityRenderer(std::shared_ptr<ShadowShader> shader, glm::mat4* projection_view_matrix) :
m_shader(std::move(shader)), m_projection_view_matrix(projection_view_matrix) {}

void ShadowMapEntityRenderer::render(const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entities_map) const {
	for(const auto& [textured_model, entities] : entities_map) {
		auto raw_model = textured_model->get_raw_model();

		bind_model(raw_model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textured_model->get_texture()->get_id());
		for(const auto& entity : entities) {
			prepare_instance(entity);
			glDrawElements(GL_TRIANGLES, raw_model->get_vertex_count(), GL_UNSIGNED_INT, nullptr);
		}
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void ShadowMapEntityRenderer::bind_model(const std::shared_ptr<RawModel>& raw_model) {
	glBindVertexArray(raw_model->get_vao_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void ShadowMapEntityRenderer::prepare_instance(const std::shared_ptr<Entity>& entity) const {
	const auto transformation_matrix = create_transformation_matrix(entity->get_position(), entity->get_rot_x(), entity->get_rot_y(), entity->get_rot_z(), entity->get_scale());
	const auto mvp_matrix = (*m_projection_view_matrix) * transformation_matrix;
	m_shader->load_mvp_matrix(mvp_matrix);
}