#pragma once

#include <vector>

#include "../renderEngine/Loader.h"
#include "GuiShader.h"
#include "GuiTexture.h"

class GuiRenderer {
public:
	GuiRenderer(Loader* loader);
	~GuiRenderer();

	void render(std::vector<GuiTexture>* guis);
private:
	std::shared_ptr<RawModel> quad;
	
	GuiShader* m_shader;
};