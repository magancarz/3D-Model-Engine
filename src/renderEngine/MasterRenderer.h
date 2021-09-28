#pragma once

#include "../Headers.h"

#include "../shaders/StaticShader.h"
#include "Renderer.h"

class MasterRenderer {
public:
	MasterRenderer();
	~MasterRenderer();

	void render(Light& sun, Camera& camera);

	void processEntity(Entity& entity);

	void cleanUp();
private:
	StaticShader m_shader;
	Renderer m_renderer;

	glm::mat4 m_projectionMatrix;

	std::map<TexturedModel*, std::vector<Entity*>*>* m_entities;

	void createProjectionMatrix();
};