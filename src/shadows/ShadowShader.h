#pragma once

#include "../shaders/ShaderProgram.h"

class ShadowShader : public ShaderProgram {
public:
	ShadowShader();

	void load_mvp_matrix(const glm::mat4& mvp_matrix) const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

private:
	int location_mvp_matrix;

};