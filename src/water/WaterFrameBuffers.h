#pragma once

#include "../Headers.h"
#include "../renderEngine/DisplayManager.h"

class WaterFrameBuffers {
public:
	WaterFrameBuffers();
	~WaterFrameBuffers();

	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();

	inline int getReflectionTexture() { return m_reflectionTexture; };
	inline int getRefractionTexture() { return m_refractionTexture; };
	inline int getRefractionDepthTexture() { return m_refractionDepthTexture; };

private:
	void initialiseReflectionFrameBuffer();
	void initialiseRefractionFrameBuffer();
	void bindFrameBuffer(int frameBuffer, int width, int height);

	int createFrameBuffer();
	int createTextureAttachment(int width, int height);
	int createDepthTextureAttachment(int width, int height);
	int createDepthBufferAttachment(int width, int height);

	static constexpr int REFLECTION_WIDTH = 320;
	static constexpr int REFLECTION_HEIGHT = 180;

	static constexpr int REFRACTION_WIDTH = 1280;
	static constexpr int REFRACTION_HEIGHT = 720;

	unsigned int m_reflectionFrameBuffer,
		m_reflectionTexture,
		m_reflectionDepthBuffer;

	unsigned int m_refractionFrameBuffer,
		m_refractionTexture,
		m_refractionDepthTexture;

};