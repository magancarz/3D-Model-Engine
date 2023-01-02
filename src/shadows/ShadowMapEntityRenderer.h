#pragma once

#include <map>
#include <vector>

#include "../entities/Entity.h"
#include "../models/TexturedModel.h"
#include "ShadowShader.h"

class ShadowMapEntityRenderer {
public:
	ShadowMapEntityRenderer(std::shared_ptr<ShadowShader> shader, glm::mat4* projection_view_matrix);

	/**
	* Renders entities to the shadow map. Each model is first bound and then all
	* of the entities using that model are rendered to the shadow map.
	*/
	void render(const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entities_map) const;

private:
	/**
	* Binds a raw model before rendering. Only the attribute 0 is enabled here
	* because that is where the positions are stored in the VAO, and only the
	* positions are required in the vertex shader.
	*/
	static void bind_model(RawModel* raw_model);

	/**
	* Prepares an entity to be rendered. The model matrix is created in the
	* usual way and then multiplied with the projection and view matrix (often
	* in the past we've done this in the vertex shader) to create the
	* mvp-matrix. This is then loaded to the vertex shader as a uniform.
	*/
	void prepare_instance(const std::shared_ptr<Entity>& entity) const;

	std::shared_ptr<ShadowShader> m_shader;

	glm::mat4* m_projection_view_matrix;
};