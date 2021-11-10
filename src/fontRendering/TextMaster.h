#pragma once

#include "../Headers.h"
#include "../renderEngine/Loader.h"

#include "FontType.h"
#include "GUIText.h"
#include "FontRenderer.h"

class TextMaster {
public:
	TextMaster();
	void init(Loader* loader);
	void loadText(GUIText* text);
	void render();
	void removeText(GUIText* text);
	void cleanUp();
private:
	Loader* loader;
	std::map<FontType*, std::vector<GUIText*>*>* texts;
	FontRenderer* renderer;
};

// global variable
extern TextMaster textMaster;