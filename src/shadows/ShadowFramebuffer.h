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
	void bind_frame_buffer() const;

	/**
	Unbinds the frame buffer, setting the default frame buffer as the current
	render target.
	*/
	static void unbind_frame_buffer();

	/**
	The ID of the shadow map texture.
	*/
	unsigned int get_shadow_map() const;

private:
	/**
	Creates the frame buffer and adds it's depth attachment texture.
	*/
	void initialise_frame_buffer();

	/**
	Binds the frame buffer as the current render target.
	*/
	static void bind_frame_buffer(unsigned int frame_buffer, int width, int height);

	/**
	Creates a frame buffer and binds it so that attachments can be added to
	it. The draw buffer is set to none, indicating that there's no color
	buffer to be rendered to.
	*/
	static unsigned int create_frame_buffer();

	/**
	Creates a depth buffer texture attachment.
	*/
	static unsigned int create_depth_buffer_attachment(int width, int height);

	const int m_width, m_height;
	unsigned int m_fbo, m_shadow_map;

};