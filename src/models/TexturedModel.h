#pragma once

#include "RawModel.h"
#include "../textures/ModelTexture.h"

class TexturedModel {
public:
	TexturedModel(RawModel model, ModelTexture texture);

	inline RawModel& getRawModel() { return m_rawModel; };
	inline ModelTexture& getTexture() { return m_texture; };
private:
	RawModel m_rawModel;
	ModelTexture m_texture;
};