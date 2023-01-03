#include "TerrainRenderer.h"

#include "MasterRenderer.h"
#include "toolbox/Maths.h"

TerrainRenderer::TerrainRenderer(const glm::mat4& projection_matrix) {
	m_terrain_shader = std::make_unique<TerrainShader>();
	m_terrain_shader->start();
	m_terrain_shader->load_projection_matrix(projection_matrix);
	m_terrain_shader->connect_texture_units();
	m_terrain_shader->stop();
}

void TerrainRenderer::render(
		const std::vector<std::shared_ptr<Terrain>>& terrains,
		const glm::mat4& to_shadow_map_space,
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const {

	m_terrain_shader->start();
	m_terrain_shader->load_clip_plane(clip_plane);
	m_terrain_shader->load_sky_color(MasterRenderer::RED, MasterRenderer::GREEN, MasterRenderer::BLUE);
	m_terrain_shader->load_lights(lights);
	m_terrain_shader->load_view_matrix(camera);
	m_terrain_shader->load_to_shadow_map_space_matrix(to_shadow_map_space);
	for(const auto& terrain : terrains) {
		prepare_terrain(terrain);
		load_model_matrix(terrain);
		glDrawElements(GL_TRIANGLES, terrain->get_model()->get_vertex_count(), GL_UNSIGNED_INT, nullptr);
		unbind_textured_model();
	}
	m_terrain_shader->stop();
}

void TerrainRenderer::prepare_terrain(const std::shared_ptr<Terrain>& terrain) const {
	const auto model = terrain->get_model();
	glBindVertexArray(model->get_vao_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	bind_textures(terrain);
	m_terrain_shader->load_shine_variables(1, 0);
}

void TerrainRenderer::bind_textures(const std::shared_ptr<Terrain>& terrain) {
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

void TerrainRenderer::unbind_textured_model() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void TerrainRenderer::load_model_matrix(const std::shared_ptr<Terrain>& terrain) const {
	const auto transformation_matrix = create_transformation_matrix(glm::vec3(terrain->get_x(), 0, terrain->get_z()), 0, 0, 0, 1);
	m_terrain_shader->load_transformation_matrix(transformation_matrix);
}