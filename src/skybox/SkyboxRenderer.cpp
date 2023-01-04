#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(const std::shared_ptr<Loader>& loader, const glm::mat4& projection_matrix) {
	m_cube = loader->load_to_vao(VERTICES, 3);
	m_texture = loader->load_cube_map(TEXTURE_FILES);
	m_night_texture = loader->load_cube_map(NIGHT_TEXTURE_FILES);

	m_skybox_shader = std::make_unique<SkyboxShader>();
	m_skybox_shader->start();
	m_skybox_shader->connect_texture_units();
	m_skybox_shader->load_projection_matrix(projection_matrix);
	m_skybox_shader->stop();
}

void SkyboxRenderer::render(const std::shared_ptr<Camera>& camera, const float r, const float g, const float b) {
	m_skybox_shader->start();
	m_skybox_shader->load_view_matrix(camera);
	m_skybox_shader->load_fog_color(r, g, b);

	glBindVertexArray(m_cube->get_vao_id());
	glEnableVertexAttribArray(0);
	bind_textures();
	glDrawArrays(GL_TRIANGLES, 0, m_cube->get_vertex_count());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	m_skybox_shader->stop();
}

void SkyboxRenderer::bind_textures() const {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_night_texture);
}