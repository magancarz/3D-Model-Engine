#pragma once

#include "../Headers.h"

#include "TextMeshData.h"
#include "TextMeshCreator.h"

/**
 * Represents a font. It holds the font's texture atlas as well as having the
 * ability to create the quad vertices for any text using this font.
 */
class FontType {
public:
	FontType(GLuint textureAtlas, std::string fontFile);
	~FontType();
	GLuint getTextureAtlas();
	TextMeshData* loadText(GUIText* text);

private:
	GLuint textureAtlas;
	TextMeshCreator* loader;
};