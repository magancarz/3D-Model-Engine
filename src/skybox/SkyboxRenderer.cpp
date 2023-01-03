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

void SkyboxRenderer::bind_textures() {
	/* code for managing time of the day */
	//m_time += display.getFrameTimeSeconds() * 100; // original: * 1000

	m_time = 1000;
	while (m_time >= 24000) {
		m_time -= 24000;
	}
	int texture1, texture2;
	float blend_factor;
	if (m_time < 5000) {
		texture1 = m_night_texture;
		texture2 = m_night_texture;
		blend_factor = (m_time - 0) / (5000 - 0);
	}
	else if (m_time >= 5000 && m_time < 8000) {
		texture1 = m_night_texture;
		texture2 = m_texture;
		blend_factor = (m_time - 5000) / (8000 - 5000);
	}
	else if (m_time >= 8000 && m_time < 21000) {
		texture1 = m_texture;
		texture2 = m_texture;
		blend_factor = (m_time - 8000) / (21000 - 8000);
	}
	else {
		texture1 = m_texture;
		texture2 = m_night_texture;
		blend_factor = (m_time - 21000) / (24000 - 21000);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
	m_skybox_shader->load_blend_factor(blend_factor);
}