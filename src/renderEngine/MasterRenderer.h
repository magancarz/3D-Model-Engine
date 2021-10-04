#pragma once

#include "../Headers.h"

#include "../shaders/StaticShader.h"
#include "../shaders/TerrainShader.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"

void enableCulling();
void disableCulling();

class MasterRenderer {
public:
	MasterRenderer();
	~MasterRenderer();

	void render(Light& sun, Camera& camera);
	void prepare();


	void processEntity(Entity& entity);
	void processTerrain(Terrain* terrain);

	void cleanUp();
private:
	StaticShader* m_shader;
	EntityRenderer* m_renderer;

	TerrainShader* m_terrainShader;
	TerrainRenderer* m_terrainRenderer;

	glm::mat4 m_projectionMatrix;
	const float FOV = 70.0f;
	const float NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;

	const float RED = 0.0f, GREEN = 0.0f, BLUE = 0.0f;

	std::map<TexturedModel*, std::vector<Entity*>*>* m_entities;
	std::vector<Terrain*>* m_terrains;

	void createProjectionMatrix();
};