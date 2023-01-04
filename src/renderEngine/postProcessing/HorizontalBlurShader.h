#pragma once

#include "../shaders/ShaderProgram.h"

class HorizontalBlurShader : public ShaderProgram {
public:
	HorizontalBlurShader();

	void load_target_width(float width) const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

private:
	int location_target_width;
};