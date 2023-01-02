#pragma once

#include <map>
#include <vector>

#include "../entities/Entity.h"
#include "../models/TexturedModel.h"
#include "ShadowShader.h"

class ShadowMapEntityRenderer {
public:
	ShadowMapEntityRenderer(ShadowShader* shader, glm::mat4* projectionViewMatrix)
		: m_shader(shader), m_projectionViewMatrix(projectionViewMatrix)  {}

	/**
	* Renders entities to the shadow map. Each model is first bound and then all
	* of the entities using that model are rendered to the shadow map.
	*/
	void render(std::map<TexturedModel*, std::vector<Entity*>*>* entitiesMap);

private:
	/**
	* Binds a raw model before rendering. Only the attribute 0 is enabled here
	* because that is where the positions are stored in the VAO, and only the
	* positions are required in the vertex shader.
	*/
	void bindModel(RawModel* rawModel);

	/**
	* Prepares an entity to be rendered. The model matrix is created in the
	* usual way and then multiplied with the projection and view matrix (often
	* in the past we've done this in the vertex shader) to create the
	* mvp-matrix. This is then loaded to the vertex shader as a uniform.
	*/
	void prepareInstance(Entity* entity);

	glm::mat4* m_projectionViewMatrix;
	ShadowShader* m_shader;
};