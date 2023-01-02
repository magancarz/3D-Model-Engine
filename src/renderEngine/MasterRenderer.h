#pragma once

#include "../skybox/SkyboxRenderer.h"
#include "../normalMappingRenderer/NormalMappingRenderer.h"
#include "../shadows/ShadowMapMasterRenderer.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "terrain/TerrainShader.h"

class MasterRenderer {
public:
	MasterRenderer(const std::shared_ptr<Loader>& loader, const std::shared_ptr<Camera>& camera);
	~MasterRenderer();

	void render(
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const;

	void render_shadow_map(
		const std::vector<std::shared_ptr<Entity>>& entity_list,
		const std::shared_ptr<Light>& sun);
	
	void prepare() const;

	static void enable_culling();
	static void disable_culling();

	void process_entity(const std::shared_ptr<Entity>& entity);
	void process_entities(const std::vector<std::shared_ptr<Entity>>& entities);
	void process_normal_map_entity(const std::shared_ptr<Entity>& entity);
	void process_terrain(const std::shared_ptr<Terrain>& terrain);

	glm::mat4 get_projection_matrix() const;
	unsigned int get_shadow_map_texture() const;

	void clean_up_objects_maps();

	inline static constexpr float RED = 0.0f, GREEN = 0.0f, BLUE = 0.0f;
	inline static constexpr float FOV = 70.0f;
	inline static constexpr float NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;

private:
	glm::mat4 m_projection_matrix;

	std::unique_ptr<StaticShader> m_static_shader;
	std::unique_ptr<NormalMappingShader> m_normal_mapped_objects_shader;
	std::unique_ptr<TerrainShader> m_terrain_shader;

	std::unique_ptr<EntityRenderer> m_entity_renderer;
	std::unique_ptr<TerrainRenderer> m_terrain_renderer;
	std::unique_ptr<SkyboxRenderer> m_skybox_renderer;
	std::unique_ptr<NormalMappingRenderer> m_normal_mapping_renderer;
	std::unique_ptr<ShadowMapMasterRenderer> m_shadow_map_renderer;

	std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>> m_entities;
	std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>> m_normal_mapped_entities;
	std::vector<std::shared_ptr<Terrain>> m_terrains;

	void create_projection_matrix();
};