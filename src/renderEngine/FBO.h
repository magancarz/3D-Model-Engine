#pragma once

#define FBO_NONE 0
#define FBO_DEPTH_TEXTURE 1
#define FBO_DEPTH_RENDER_BUFFER 2

class FBO {
public:
	/// <summary>
	/// Creates an FBO of a specified width and height, with the desired type of 
	/// depth buffer attachment.
	/// </summary>
	/// <param name="width"> - the width of the FBO</param>
	/// <param name="height"> - the height of the FBO</param>
	/// <param name="depthBufferType"> - an int indicating the type of depth buffer 
	/// attachment that this FBO should use</param>
	FBO(unsigned int width, unsigned int height, unsigned int depthBufferType);

	FBO(unsigned int width, unsigned int height);

	/// <summary>
	/// Deletes the frame buffer and its attachments when the program closes.
	/// </summary>
	void cleanUp();

	/// <summary>
	/// Binds the frame buffer, setting it as the current render target. Anything
	/// rendered after this will be rendered to this FBO, and not to the screen.
	/// </summary>
	void bindFrameBuffer();

	/// <summary>
	/// Unbinds the frame buffer, setting the default frame buffer as the current
	/// render target. Anything rendered after this will be rendered to the
	/// screen, and not this FBO.
	/// </summary>
	void unbindFrameBuffer();

	/// <summary>
	/// Binds the current FBO to be read from.
	/// </summary>
	void bindToRead();

	void resolveToFBO(FBO* outputFBO);

	void resolveToScreen();

	inline unsigned int getColorTexture() { return m_colorTexture; }
	inline unsigned int getDepthTexture() { return m_depthTexture; }

private:
	/// <summary>
	/// Creates the FBO along with a color buffer texture attachment, and
	/// possibly a depth buffer.
	/// </summary>
	/// <param name="type"> - the type of depth buffer attachment to be attached to the FBO</param>
	void initialiseFrameBuffer(unsigned int type);

	/// <summary>
	/// Creates a new frame buffer object and sets the buffer to which drawing
	/// will occur - color attachment 0. This is the attachment where the color
	/// buffer texture is.
	/// </summary>
	void createFrameBuffer();

	/// <summary>
	/// Creates a texture and sets it as the color buffer attachment for this FBO.
	/// </summary>
	void createTextureAttachment();

	/// <summary>
	/// Adds a depth buffer to the FBO in the form of a texture, which can later
	/// be sampled.
	/// </summary>
	void createDepthTextureAttachment();

	void createMultisampleColorAttachment();

	/// <summary>
	/// Adds a depth buffer to the FBO in the form of a render buffer. This can't
	/// be used for sampling in the shaders.
	/// </summary>
	void createDepthBufferAttachment();

	const unsigned int m_width = 0, m_height = 0;

	unsigned int m_frameBuffer, m_depthBuffer, m_colorBuffer;

	unsigned int m_colorTexture, m_depthTexture;

	bool m_multisample = false;
};