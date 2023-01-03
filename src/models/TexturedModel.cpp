#include "TexturedModel.h"

TexturedModel::TexturedModel(std::shared_ptr<RawModel> model, std::shared_ptr<ModelTexture> texture ) :
m_raw_model(std::move(model)), m_texture(std::move(texture)) {}

std::shared_ptr<RawModel> TexturedModel::get_raw_model() {
	return m_raw_model;
}

std::shared_ptr<ModelTexture> TexturedModel::get_texture() {
	return m_texture;
}