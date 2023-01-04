#include "MasterRenderer.h"

#include "toolbox/DisplayManager.h"

MasterRenderer::MasterRenderer(const std::shared_ptr<Loader>& loader, const std::shared_ptr<Camera>& camera) {
	create_projection_matrix();

	m_entity_renderer = std::make_unique<EntityRenderer>(m_projection_matrix);
	m_terrain_renderer = std::make_unique<TerrainRenderer>(m_projection_matrix);
	m_skybox_renderer = std::make_unique<SkyboxRenderer>(loader, m_projection_matrix);
	m_normal_mapping_renderer = std::make_unique<NormalMappingRenderer>(m_projection_matrix);
}

void MasterRenderer::render(
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const {

	prepare();

	m_entity_renderer->render(m_entities, lights, camera, clip_plane);

	m_normal_mapping_renderer->render(m_normal_mapped_entities, lights, camera, clip_plane);

	m_terrain_renderer->render(m_terrains, lights, camera, clip_plane);

	m_skybox_renderer->render(camera, RED, GREEN, BLUE);
}

void MasterRenderer::prepare() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(RED, GREEN, BLUE, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::enable_culling() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MasterRenderer::disable_culling() {
	glDisable(GL_CULL_FACE);
}

void MasterRenderer::process_entities(const std::vector<std::shared_ptr<Entity>>& entities) {
	for(const auto& entity : entities) {
		process_entity(entity);
	}
}

void MasterRenderer::process_entity(const std::shared_ptr<Entity>& entity) {
	auto entity_model = entity->get_textured_model();

	const auto it = m_entities.find(entity_model);
	if(it != m_entities.end()) {
		auto& batch = it->second;
		batch.push_back(entity);
	} else {
		std::vector<std::shared_ptr<Entity>> new_batch;
		new_batch.push_back(entity);
		m_entities.insert(std::make_pair(entity_model, new_batch));
	}
}

void MasterRenderer::process_normal_map_entity(const std::shared_ptr<Entity>& entity) {
	auto entity_model = entity->get_textured_model();

	const auto it = m_normal_mapped_entities.find(entity_model);
	if(it != m_normal_mapped_entities.end()) {
		auto& batch = it->second;
		batch.push_back(entity);
	} else {
		std::vector<std::shared_ptr<Entity>> new_batch;
		new_batch.push_back(entity);
		m_normal_mapped_entities.insert(std::make_pair(entity_model, new_batch));
	}
}

void MasterRenderer::process_terrain(const std::shared_ptr<Terrain>& terrain) {
	m_terrains.push_back(terrain);
}

glm::mat4 MasterRenderer::get_projection_matrix() const { return m_projection_matrix; }

void MasterRenderer::clean_up_objects_maps() {
	m_entities.clear();
	m_normal_mapped_entities.clear();
	m_terrains.clear();
}

void MasterRenderer::create_projection_matrix() {
	m_projection_matrix = glm::mat4();
	constexpr float aspect_ratio = static_cast<float>(DisplayManager::WINDOW_WIDTH) / static_cast<float>(DisplayManager::WINDOW_HEIGHT);
	const float y_scale = 1.0f / glm::tan(glm::radians(FOV / 2.0f));
	const float x_scale = y_scale / aspect_ratio;
	constexpr float frustum_length = FAR_PLANE - NEAR_PLANE;

	m_projection_matrix[0][0] = x_scale;
	m_projection_matrix[1][1] = y_scale;
	m_projection_matrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	m_projection_matrix[2][3] = -1;
	m_projection_matrix[3][2] = -((2.0f * NEAR_PLANE * FAR_PLANE) / frustum_length);
	m_projection_matrix[3][3] = 0;
}