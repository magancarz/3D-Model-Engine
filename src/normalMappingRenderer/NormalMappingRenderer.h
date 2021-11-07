#pragma once

#include "../shaders/NormalMappingShader.h"
#include "../models/TexturedModel.h"
#include "../entities/Entity.h"
#include "../entities/Camera.h"

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