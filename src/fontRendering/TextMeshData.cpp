#include "TextMeshData.h"

TextMeshData::TextMeshData(
	std::vector<GLfloat> vertexPositions,
	std::vector<GLfloat> textureCoords)
{
	this->vertexPositions = vertexPositions;
	this->textureCoords = textureCoords;
}

std::vector<GLfloat>& TextMeshData::getVertexPositions()
{
	return vertexPositions;
}

std::vector<GLfloat>& TextMeshData::getTextureCoords()
{
	return textureCoords;
}

int TextMeshData::getVertexCount()
{
	return vertexPositions.size() / 2;
}
