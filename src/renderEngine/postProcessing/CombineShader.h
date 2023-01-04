#pragma once

#include "../shaders/ShaderProgram.h"

class CombineShader : public ShaderProgram {
public:
	CombineShader();

	void connect_texture_units() const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

private:
	int location_color_texture,
		location_highlight_texture;

};
