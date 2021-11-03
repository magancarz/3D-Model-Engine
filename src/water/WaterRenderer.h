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

	static constexpr float WAVE_SPEED = 0.3f;

	RawModel* quad;
	WaterShader* m_shader;
	WaterFrameBuffers* m_fbos;

	unsigned int m_dudvTexture;
	unsigned float moveFactor = 0;
};