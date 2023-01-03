#pragma once

#include "../models/RawModel.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"
#include "WaterShader.h"
#include "renderEngine/Loader.h"

class WaterRenderer {
public:
	WaterRenderer(
		const std::shared_ptr<Loader>& loader,
		std::shared_ptr<WaterFrameBuffers> water_frame_buffers,
		const glm::mat4& projection);

	void render(
		const std::vector<std::shared_ptr<WaterTile>>& water_tiles,
		const std::shared_ptr<Camera>& camera,
		const std::shared_ptr<Light>& sun);

private:
	void prepare_render(
		const std::shared_ptr<Camera>& camera,
		const std::shared_ptr<Light>& sun);

	void set_up_vao(const std::shared_ptr<Loader>& loader);

	void unbind() const;

	static constexpr float WAVE_SPEED = 0.05f;

	std::shared_ptr<RawModel> m_quad;
	std::unique_ptr<WaterShader> m_water_shader;
	std::shared_ptr<WaterFrameBuffers> m_water_frame_buffers;

	unsigned int m_dudv_texture, m_normal_map;

	float m_move_factor = 0;
};
