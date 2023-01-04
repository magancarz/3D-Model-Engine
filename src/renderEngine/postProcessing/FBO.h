#pragma once

#include <memory>

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
	/// <param name="depth_buffer_type"> - an int indicating the type of depth buffer 
	/// attachment that this FBO should use</param>
	FBO(int width, int height, unsigned int depth_buffer_type);

	FBO(int width, int height);

	/// <summary>
	/// Deletes the frame buffer and its attachments when the program closes.
	/// </summary>
	void clean_up() const;

	/// <summary>
	/// Binds the frame buffer, setting it as the current render target. Anything
	/// rendered after this will be rendered to this FBO, and not to the screen.
	/// </summary>
	void bind_frame_buffer() const;

	/// <summary>
	/// Unbinds the frame buffer, setting the default frame buffer as the current
	/// render target. Anything rendered after this will be rendered to the
	/// screen, and not this FBO.
	/// </summary>
	static void unbind_frame_buffer();

	/// <summary>
	/// Binds the current FBO to be read from.
	/// </summary>
	void bind_to_read() const;

	void resolve_to_fbo(unsigned int read_buffer, const std::unique_ptr<FBO>& output_fbo) const;

	void resolve_to_screen() const;

	unsigned int get_color_texture() const;
	unsigned int get_depth_texture() const;

private:
	/// <summary>
	/// Creates the FBO along with a color buffer texture attachment, and
	/// possibly a depth buffer.
	/// </summary>
	/// <param name="type"> - the type of depth buffer attachment to be attached to the FBO</param>
	void initialise_frame_buffer(unsigned int type);

	/// <summary>
	/// Creates a new frame buffer object and sets the buffer to which drawing
	/// will occur - color attachment 0. This is the attachment where the color
	/// buffer texture is.
	/// </summary>
	void create_frame_buffer();

	void determine_draw_buffers() const;

	/// <summary>
	/// Creates a texture and sets it as the color buffer attachment for this FBO.
	/// </summary>
	void create_texture_attachment();

	/// <summary>
	/// Adds a depth buffer to the FBO in the form of a texture, which can later
	/// be sampled.
	/// </summary>
	void create_depth_texture_attachment();

	unsigned int create_multi_sample_color_attachment(int attachment) const;

	/// <summary>
	/// Adds a depth buffer to the FBO in the form of a render buffer. This can't
	/// be used for sampling in the shaders.
	/// </summary>
	void create_depth_buffer_attachment();

	const int m_width = 0, m_height = 0;

	unsigned int m_frame_buffer, m_depth_buffer,
				 m_color_buffer1, m_color_buffer2;

	unsigned int m_color_texture, m_depth_texture;

	bool m_multi_sample_and_multi_target = false;
};
