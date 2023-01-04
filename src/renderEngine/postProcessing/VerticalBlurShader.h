#pragma once

#include "../shaders/ShaderProgram.h"

class VerticalBlurShader : public ShaderProgram {
public:
	VerticalBlurShader();

	void load_target_height(float width) const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

private:
	int location_target_height;
};
