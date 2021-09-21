#pragma once

#include "../Headers.h"

#include "../entities/Entity.h"
#include "../models/RawModel.h"
#include "../models/TexturedModel.h"
#include "../shaders/StaticShader.h"
#include "../toolbox/Maths.h"
#include "../renderEngine/DisplayManager.h"

class Renderer {
public:
	Renderer(StaticShader& shader);

	void prepare();
	void render(const Entity& entity, StaticShader& shader);
private:
	void createProjectionMatrix();

	glm::mat4 m_projectionMatrix;
	const float FOV = 70.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;
};