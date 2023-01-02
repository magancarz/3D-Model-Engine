#include "ShadowMapMasterRenderer.h"

ShadowMapMasterRenderer::ShadowMapMasterRenderer(const std::shared_ptr<Camera>& camera) {
	m_shader = std::make_shared<ShadowShader>();
	m_shadow_box = std::make_unique<ShadowBox>(&m_light_view_matrix, camera);
	m_shadow_fbo = std::make_unique<ShadowFrameBuffer>(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	m_shadow_map_entity_renderer = std::make_unique<ShadowMapEntityRenderer>(m_shader, &m_projection_view_matrix);

	m_offset = create_offset();
}

void ShadowMapMasterRenderer::render(
		const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entities_map,
		const std::shared_ptr<Light>& sun) {

	m_shadow_box->update();

	const auto sun_position = sun->get_position();
	const auto light_direction = glm::vec3(sun_position.x, sun_position.y, sun_position.z);
	prepare(light_direction);

	m_shadow_map_entity_renderer->render(entities_map);

	finish();
}

glm::mat4 ShadowMapMasterRenderer::get_to_shadow_map_space_matrix() const { return m_offset * m_projection_view_matrix; }

unsigned int ShadowMapMasterRenderer::get_shadow_map() const { return m_shadow_fbo->get_shadow_map(); }

glm::mat4 ShadowMapMasterRenderer::get_light_space_transform() const { return m_light_view_matrix; }

void ShadowMapMasterRenderer::prepare(const glm::vec3& light_direction) {
	update_ortho_projection_matrix(m_shadow_box->get_width(), m_shadow_box->get_height(), m_shadow_box->get_length());
	update_light_view_matrix(light_direction, m_shadow_box->get_center());
	m_projection_view_matrix = m_offset * m_projection_matrix * m_light_view_matrix;
	m_shadow_fbo->bind_frame_buffer();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shader->start();
}

void ShadowMapMasterRenderer::finish() const {
	m_shader->stop();
	m_shadow_fbo->unbind_frame_buffer();
}

void ShadowMapMasterRenderer::update_light_view_matrix(const glm::vec3& direction, const glm::vec3& center) {
	m_light_view_matrix = glm::lookAt(direction, glm::vec3(400.f, 0.f, 400.f), glm::vec3(0.f, 1.f, 0.f));
}

void ShadowMapMasterRenderer::update_ortho_projection_matrix(float width, float height, float length) {
	/*m_projection_matrix = glm::mat4(1.0f);
	m_projection_matrix[0][0] = 2.0f / width;
	m_projection_matrix[1][1] = 2.0f / height;
	m_projection_matrix[2][2] = -2.0f / length;
	m_projection_matrix[3][3] = 1;*/
	constexpr float ortho_size = 128;
	m_projection_matrix = glm::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, 0.1f, 100000.f);
}

glm::mat4 ShadowMapMasterRenderer::create_offset() {
	auto offset = glm::mat4(1.f);
	offset = glm::translate(offset, glm::vec3(0.5f, 0.5f, 0.5f));
	offset = glm::scale(offset, glm::vec3(0.5f, 0.5f, 0.5f));
	return offset;
}