#pragma once

#include <vector>
#include <map>

#include "NormalMappingShader.h"
#include "../models/TexturedModel.h"
#include "../entities/Entity.h"
#include "../entities/Camera.h"
#include "entities/Light.h"

class NormalMappingRenderer {
public:
	NormalMappingRenderer(glm::mat4 proj);
	~NormalMappingRenderer();

	void render(std::map<TexturedModel*, std::vector<Entity*>*>* entities, glm::vec4 clipPlane, std::vector<Light*>& lights, Camera& camera);

private:
	void prepareTexturedModel(TexturedModel* model);
	void unbindTexturedModel();
	void prepareInstance(Entity* entity);
	void prepare(glm::vec4 clipPlane, std::vector<Light*>& lights, Camera& camera);

	NormalMappingShader* m_shader;

};
