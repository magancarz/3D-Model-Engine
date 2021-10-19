#pragma once

#include "../renderEngine/Loader.h"
#include "../shaders/GuiShader.h"
#include "GuiTexture.h"

class GuiRenderer {
public:
	GuiRenderer(Loader* loader);
	~GuiRenderer();

	void render(std::vector<GuiTexture>* guis);
private:
	const RawModel* quad;
	
	GuiShader* m_shader;
};