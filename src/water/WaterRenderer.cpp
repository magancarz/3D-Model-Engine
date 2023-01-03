#include "WaterRenderer.h"

#include "toolbox/Maths.h"

WaterRenderer::WaterRenderer(
		const std::shared_ptr<Loader>& loader,
		std::shared_ptr<WaterFrameBuffers> water_frame_buffers,
		const glm::mat4& projection) :
m_water_frame_buffers(std::move(water_frame_buffers)) {
	
	m_water_shader = std::make_unique<WaterShader>();
	m_water_shader->start();
	m_water_shader->connect_texture_units();
	m_water_shader->load_projection_matrix(projection);
	m_water_shader->stop();

	m_dudv_texture = loader->loadTexture("res/textures/waterDUDV.png");
	m_normal_map = loader->loadTexture("res/textures/matchingNormalMap.png");

	set_up_vao(loader);
}

void WaterRenderer::render(
		const std::vector<std::shared_ptr<WaterTile>>& water_tiles,
		const std::shared_ptr<Camera>& camera,
		const std::shared_ptr<Light>& sun) {

	prepare_render(camera, sun);

	for(const auto& water_tile : water_tiles) {
		const glm::mat4 model_matrix = create_transformation_matrix(
			glm::vec3(water_tile->get_x(),
			water_tile->get_height(),
			water_tile->get_z()),
			0, 0, 0,
			WaterTile::WATER_TILE_SIZE);
		m_water_shader->load_model_matrix(model_matrix);
		glDrawArrays(GL_TRIANGLES, 0, m_quad->getVertexCount());
	}
	unbind();
}

void WaterRenderer::prepare_render(
		const std::shared_ptr<Camera>& camera,
		const std::shared_ptr<Light>& sun) {

	m_water_shader->start();
	m_water_shader->load_view_matrix(camera);
	m_move_factor += WAVE_SPEED * DisplayManager::getFrameTimeSeconds();
	m_move_factor = fmod(m_move_factor, 1.0);
	m_water_shader->load_move_factor(m_move_factor);
	m_water_shader->load_light(sun);

	glBindVertexArray(m_quad->getVaoID());
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_water_frame_buffers->get_reflection_texture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_water_frame_buffers->get_refraction_texture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_dudv_texture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_normal_map);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_water_frame_buffers->get_refraction_depth_texture());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::unbind() const {
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_water_shader->stop();
}

void WaterRenderer::set_up_vao(const std::shared_ptr<Loader>& loader) {
	//Just x and z vertex positions here, y is set to 0 in v.shader
	const std::vector<float> vertices = {
		-1, -1, -1,
		 1,  1, -1,
		 1, -1, -1,
		 1,  1,  1
	};
	m_quad = loader->loadToVAO(vertices, 2);
}