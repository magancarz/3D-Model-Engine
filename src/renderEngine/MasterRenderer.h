#pragma once

#include "../skybox/SkyboxRenderer.h"
#include "../entities/EntityRenderer.h"
#include "../entities/Light.h"
#include "../entities/NormalMappingRenderer.h"
#include "../entities/NormalMappingShader.h"
#include "../models/Loader.h"
#include "../terrain/TerrainRenderer.h"

class MasterRenderer {
public:
	MasterRenderer(const std::shared_ptr<Loader>& loader, const std::shared_ptr<Camera>& camera);

	void render(
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const;
	
	static void prepare();

	static void enable_culling();
	static void disable_culling();

	void process_entity(const std::shared_ptr<Entity>& entity);
	void process_entities(const std::vector<std::shared_ptr<Entity>>& entities);

	void process_normal_map_entity(const std::shared_ptr<Entity>& entity);
	void process_normal_map_entities(const std::vector<std::shared_ptr<Entity>>& entities);
	void process_terrain(const std::shared_ptr<Terrain>& terrain);

	glm::mat4 get_projection_matrix() const;

	void clean_up_objects_maps();

	inline static constexpr float RED = 0.0f, GREEN = 0.0f, BLUE = 0.0f;
	inline static constexpr float FOV = 70.0f;
	inline static constexpr float NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;

private:
	glm::mat4 m_projection_matrix;
	
	std::unique_ptr<NormalMappingShader> m_normal_mapped_objects_shader;

	std::unique_ptr<EntityRenderer> m_entity_renderer;
	std::unique_ptr<TerrainRenderer> m_terrain_renderer;
	std::unique_ptr<SkyboxRenderer> m_skybox_renderer;
	std::unique_ptr<NormalMappingRenderer> m_normal_mapping_renderer;

	std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>> m_entities;
	std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>> m_normal_mapped_entities;
	std::vector<std::shared_ptr<Terrain>> m_terrains;

	void create_projection_matrix();
};
