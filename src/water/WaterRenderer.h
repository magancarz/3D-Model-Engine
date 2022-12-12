#pragma once

#include "../models/RawModel.h"
#include "../shaders/WaterShader.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

class WaterRenderer {
public:
	WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projection, WaterFrameBuffers* fbos);

	void render(std::vector<WaterTile*>& water, Camera& camera, Light& sun);
private:
	void prepareRender(Camera& camera, Light& sun);
	void unbind();
	void setUpVAO(Loader& loader);

	static constexpr float WAVE_SPEED = 0.05f;

	RawModel* quad;
	WaterShader* m_shader;
	WaterFrameBuffers* m_fbos;

	unsigned int m_dudvTexture;
	unsigned int m_normalMap;
	float moveFactor = 0;
};