#pragma once

#include <memory>

#include "RawModel.h"
#include "../textures/ModelTexture.h"

class TexturedModel {
public:
	TexturedModel(std::shared_ptr<RawModel> model, std::shared_ptr<ModelTexture> texture);

	std::shared_ptr<RawModel> get_raw_model();
	std::shared_ptr<ModelTexture> get_texture();

private:
	std::shared_ptr<RawModel> m_raw_model;
	std::shared_ptr<ModelTexture> m_texture;
};