#pragma once

#include <map>
#include <vector>

#include "StaticShader.h"
#include "../entities/Entity.h"
#include "../models/TexturedModel.h"

extern void enable_culling();
extern void disable_culling();

class EntityRenderer {
public:
	EntityRenderer();
	EntityRenderer(StaticShader* shader, glm::mat4 projectionMatrix);

	void render(std::map<TexturedModel*, std::vector<Entity*>*>* entities);

	inline void setShader(StaticShader& shader) { m_shader = &shader; };
private:
	void prepareTexturedModel(TexturedModel& texturedModel);
	void unbindTexturedModel();
	void prepareInstance(Entity& entity);

	StaticShader* m_shader;

	glm::mat4 m_projectionMatrix;
	const float FOV = 70.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;
};