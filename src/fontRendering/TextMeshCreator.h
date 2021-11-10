#pragma once

#include "../Headers.h"

#include "GUIText.h"
#include "TextMeshData.h"
#include "MetaFile.h"
#include "Line.h"

class TextMeshCreator {
public:
	static constexpr double LINE_HEIGHT = 0.03f;
	static constexpr int SPACE_ASCII = 32;
  
	TextMeshCreator(std::string metaFile);
	TextMeshData* createTextMesh(GUIText* text);
private:
	std::vector<Line*>* createStructure(GUIText *text);
	void completeStructure(std::vector<Line*>* lines, Line* currentLine,
		Word* currentWord, GUIText* text);
	TextMeshData* createQuadVertices(GUIText* text, std::vector<Line*>* lines);
	void addVerticesForCharacter(double cursorX, double cursorY, 
		Character character, double fontSize,
		std::vector<GLfloat>& vertices);
	void addVertices(std::vector<GLfloat>& vertices, double x, double y, double maxX, double maxY);
	void addTexCoords(std::vector<GLfloat>& texCoords, double x, double y, double maxX, double maxY);

	MetaFile *metaData;
};