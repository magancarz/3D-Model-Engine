#pragma once

#include "RawModel.h"
#include "../textures/ModelTexture.h"

class TexturedModel {
public:
	TexturedModel(RawModel model, ModelTexture texture);

	inline RawModel getRawModel() const { return m_rawModel; };
	inline ModelTexture getTexture() const { return m_texture; };
private:
	RawModel m_rawModel;
	ModelTexture m_texture;
};