#pragma once

#include "../Headers.h"

/**
 * Stores the vertex data for all the quads on which a text will be rendered.
 */
class TextMeshData {
public:
	TextMeshData(std::vector<GLfloat> vertexPositions,
		     std::vector<GLfloat> textureCoords);

	std::vector<GLfloat>& getVertexPositions();
	std::vector<GLfloat>& getTextureCoords();

	int getVertexCount();
	
private:
	std::vector<GLfloat> vertexPositions;
	std::vector<GLfloat> textureCoords;
};