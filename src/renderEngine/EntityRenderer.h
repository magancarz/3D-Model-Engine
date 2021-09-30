#pragma once

#include "../Headers.h"

#include "../entities/Entity.h"
#include "../models/RawModel.h"
#include "../models/TexturedModel.h"
#include "../shaders/StaticShader.h"
#include "../toolbox/Maths.h"
#include "../renderEngine/DisplayManager.h"

class EntityRenderer {
public:
	EntityRenderer();
	EntityRenderer(StaticShader& shader, glm::mat4 projectionMatrix);

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