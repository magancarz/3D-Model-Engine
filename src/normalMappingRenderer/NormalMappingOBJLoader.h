#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "../models/RawModel.h"
#include "../renderEngine/Loader.h"
#include "VertexNM.h"

class NormalMappingOBJLoader {
public:
	static RawModel* loadNormalMappedOBJ(std::string objFileName, Loader& loader);
	static VertexNM* processVertex(
		int index,
		int textureIndex,
		int normalIndex,
		std::vector<VertexNM*>& vertices,
		std::vector<GLuint>& indices);

	static void calculateTangents(
		VertexNM* v0, VertexNM* v1, VertexNM* v2,
		std::vector<glm::vec2>& textures);

	static GLfloat convertDataToArrays(
		std::vector<VertexNM*>& vertices,
		std::vector<glm::vec2>& textures,
		std::vector<glm::vec3>& normals,
		std::vector<GLfloat>& verticesArray,
		std::vector<GLfloat>& texturesArray,
		std::vector<GLfloat>& normalsArray,
		std::vector<GLfloat>& tangentsArray
		);

	static VertexNM* dealWithAlreadyProcessedVertex(
		VertexNM *previousVertex,
		int newTextureIndex,
		int newNormalIndex,
		std::vector<GLuint>& indices,
		std::vector<VertexNM*>& vertices);

	static void removeUnusedVertices(std::vector<VertexNM*>& vertices);
};