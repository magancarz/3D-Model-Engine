#pragma once

#include "../Headers.h"

#include "FontType.h"
#include "GUIText.h"
#include "FontShader.h"


class FontRenderer {
public:
	FontRenderer();
	~FontRenderer();
	void render(std::map<FontType*, std::vector<GUIText*>*>* texts);
	void cleanUp();
	void prepare();
	void renderText(GUIText* text);
	void endRendering();
private:
	FontShader *shader;
};