#pragma once

#include "../renderEngine/postProcessing/FBO.h"
#include "../toolbox/DisplayManager.h"

class WaterFrameBuffers {
public:
	WaterFrameBuffers();
	~WaterFrameBuffers();

	void bind_reflection_frame_buffer() const;
	void bind_refraction_frame_buffer() const;
	static void unbind_current_frame_buffer();

	unsigned int get_reflection_texture() const;
	unsigned int get_refraction_texture() const;
	unsigned int get_refraction_depth_texture() const;

private:
	void initialise_reflection_frame_buffer();
	void initialise_refraction_frame_buffer();
	static void bind_frame_buffer(unsigned int frame_buffer, int width, int height);

	static unsigned int create_frame_buffer();
	static unsigned int create_texture_attachment(int width, int height);
	static unsigned int create_depth_texture_attachment(int width, int height);
	static unsigned int create_depth_buffer_attachment(int width, int height);

	static constexpr int REFLECTION_WIDTH = DisplayManager::WINDOW_WIDTH;
	static constexpr int REFLECTION_HEIGHT = DisplayManager::WINDOW_HEIGHT;

	static constexpr int REFRACTION_WIDTH = DisplayManager::WINDOW_WIDTH;
	static constexpr int REFRACTION_HEIGHT = DisplayManager::WINDOW_HEIGHT;

	unsigned int m_reflection_frame_buffer,
				 m_reflection_texture,
				 m_reflection_depth_buffer;

	unsigned int m_refraction_frame_buffer,
				 m_refraction_texture,
				 m_refraction_depth_texture;

};
