#pragma once

#include <map>
#include <vector>

#include "StaticShader.h"
#include "../entities/Entity.h"
#include "../models/TexturedModel.h"

class EntityRenderer {
public:
	EntityRenderer(const glm::mat4& projection_matrix);

	void render(
		const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entity_map,
		const std::vector<std::shared_ptr<Light>>& lights,
		const std::shared_ptr<Camera>& camera,
		const glm::vec4& clip_plane) const;
		
private:
	void prepare_textured_model(const std::shared_ptr<TexturedModel>& textured_model) const;
	void prepare_instance(const std::shared_ptr<Entity>& entity) const;

	static void unbind_textured_model();

	std::unique_ptr<StaticShader> m_static_shader;
};