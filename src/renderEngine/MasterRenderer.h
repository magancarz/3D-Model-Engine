#pragma once

#include "../Headers.h"
#include "../shaders/StaticShader.h"
#include "../shaders/TerrainShader.h"
#include "../skybox/SkyboxRenderer.h"
#include "../normalMappingRenderer/NormalMappingRenderer.h"
#include "../shadows/ShadowMapMasterRenderer.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"

const float RED = 0.0f, GREEN = 0.0f, BLUE = 0.0f;
const float FOV = 70.0f;
const float NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;

class MasterRenderer {
public:
	MasterRenderer(Loader* loader, Camera* camera);
	~MasterRenderer();

	void render(std::vector<Light*>& lights, Camera& camera, glm::vec4 clipPlane);
	void renderShadowMap(std::vector<Entity*>* entityList, Light* sun);
	
	void prepare();

	static void enable_culling();
	static void disable_culling();

	void processEntity(Entity& entity);
	void processEntities(std::vector<Entity*>* entityList);
	void processNormalMapEntity(Entity& entity);
	void processTerrain(Terrain* terrain);

	inline glm::mat4 getProjectionMatrix() { return m_projectionMatrix; };
	inline unsigned int getShadowMapTexture() { return m_shadowMapRenderer->getShadowMap(); }

	void cleanUp();
private:
	StaticShader* m_shader;
	NormalMappingShader* m_normalMappingShader;
	EntityRenderer* m_renderer;

	TerrainShader* m_terrainShader;
	TerrainRenderer* m_terrainRenderer;

	SkyboxRenderer* m_skyboxRenderer;

	NormalMappingRenderer* m_normalMappingRenderer;

	ShadowMapMasterRenderer* m_shadowMapRenderer;

	glm::mat4 m_projectionMatrix;

	std::map<TexturedModel*, std::vector<Entity*>*>* m_entities;
	std::map<TexturedModel*, std::vector<Entity*>*>* m_normalMappingEntities;
	std::vector<Terrain*>* m_terrains;

	void createProjectionMatrix();
};