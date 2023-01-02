#pragma once

class ShadowFrameBuffer {
public:
	/**
	Initialises the frame buffer and shadow map of a certain size.
	*/
	ShadowFrameBuffer(int width, int height);

	/**
	Deletes the frame buffer and shadow map texture when the game closes.
	*/
	~ShadowFrameBuffer();

	/**
	Binds the frame buffer, setting it as the current render target.
	*/
	void bindFrameBuffer();

	/**
	Unbinds the frame buffer, setting the default frame buffer as the current
	render target.
	*/
	void unbindFrameBuffer();

	/**
	The ID of the shadow map texture.
	*/
	unsigned int getShadowMap() { return m_shadowMap; }

private:
	/**
	Creates the frame buffer and adds it's depth attachment texture.
	*/
	void initialiseFrameBuffer();

	/**
	Binds the frame buffer as the current render target.
	*/
	void bindFrameBuffer(unsigned int frameBuffer, int width, int height);

	/**
	Creates a frame buffer and binds it so that attachments can be added to
	it. The draw buffer is set to none, indicating that there's no color
	buffer to be rendered to.
	*/
	unsigned int createFrameBuffer();

	/**
	Creates a depth buffer texture attachment.
	*/
	unsigned int createDepthBufferAttachment(int width, int height);

	const int WIDTH, HEIGHT;
	unsigned int m_fbo, m_shadowMap;

};