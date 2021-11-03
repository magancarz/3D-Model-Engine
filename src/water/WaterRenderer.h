#pragma once

#include "../models/RawModel.h"
#include "../shaders/WaterShader.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

class WaterRenderer {
public:
	WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projection, WaterFrameBuffers* fbos);

	void render(std::vector<WaterTile*>& water, Camera& camera);
private:
	void prepareRender(Camera& camera);
	void unbind();
	void setUpVAO(Loader& loader);

	RawModel* quad;
	WaterShader* m_shader;
	WaterFrameBuffers* m_fbos;
};