#include "NormalMappingObjLoader.h"

RawModel* NormalMappingOBJLoader::loadNormalMappedOBJ(std::string objFileName, Loader& loader)
{
	std::ifstream inFile(objFileName, std::ios::in);

	if (!inFile) {
		std::cerr << "NormalMappingObjLoader: File " << objFileName << " could not be opened" << std::endl;
		exit(1);
	}

	std::string line;
	std::vector<VertexNM*> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<GLuint> indices;

	// This loop collects the vertices, texture coords and normals from
	// the obj file.
	while ( !inFile.eof() ) {
		getline(inFile, line);

		std::istringstream iss(line);
		std::string starts;
		GLfloat x, y, z;

		// starts contains e.g. v, vt, tv, s, f
		iss >> starts;

		if (starts == "v") {
			// e.g. v 3.227124 -0.065127 -1.000000
			iss >> x >> y >> z;
			// cout << "got: v " << setprecision(8) << x << " " << y << " " << z << endl;
			glm::vec3 vertex(x, y, z);
			VertexNM* newVertex = new VertexNM(vertices.size(), vertex);
			vertices.push_back(newVertex);
		}
		else if (starts == "vt") {
			// e.g. vt 0.905299 0.942320
			iss >> x >> y;
			// cout << "got: vt " << setprecision(8) << x << " " << y << endl;
			glm::vec2 texture(x, y);
			textures.push_back(texture);
		}
		else if (starts == "vn") {
			// e.g. vn -1.000000 0.000000 0.000000
			iss >> x >> y >> z;
			// cout << "got: vn " << setprecision(8) << x << " " << y << " " << z << endl;
			glm::vec3 normal(x, y, z);
			normals.push_back(normal);
		}
		else if (starts == "f") {
			// break when faces start
			//cout << "NormalMappingObjLoader: Read " << vertices.size() << " vertices from " << fileName << endl;
			//cout << "NormalMappingObjLoader: Read " << textures.size() << " texture coords from " << fileName << endl;
			//cout << "NormalMappingObjLoader: Read " << normals.size() << " normals from " << fileName << endl;
			break;
		}
	}

	int faces = 0;

	// read the faces in a second loop
	while ( !inFile.eof() ) {
		if (line == "")
			break;

		std::istringstream iss(line);
		std::string starts;
		GLuint u[9];

		iss >> starts;

		if (starts == "f") {
			// e.g. f 41/1/1 38/2/1 45/3/1
			std::string tmp, f = "";
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp;

			// replace /'s with space.
			size_t x = f.find("/");
			while (x < std::string::npos) {
				f.replace (x, 1, " ");
				x = f.find("/", x + 1);
			}
			std::istringstream iss2(f);
			for (int i = 0; i < 9; i++) {
				iss2 >> u[i];
				// the indices in the obj file start from 1, ours start from 0
				u[i]--;
			}

			VertexNM* v0 = processVertex(u[0], u[1], u[2], vertices, indices);
			VertexNM* v1 = processVertex(u[3], u[4], u[5], vertices, indices);
			VertexNM* v2 = processVertex(u[6], u[7], u[8], vertices, indices);
			calculateTangents(v0, v1, v2, textures);

			faces++;
		}

		getline(inFile, line);
	}

	//cout << "NormalMappingObjLoader: Read " << faces << " faces from " << fileName << endl;

	removeUnusedVertices(vertices);
	std::vector<GLfloat> verticesArray;
	std::vector<GLfloat> texturesArray;
	std::vector<GLfloat> normalsArray;
	std::vector<GLfloat> tangentsArray;
	GLfloat furthest = convertDataToArrays(
		vertices, textures, normals,
		verticesArray, texturesArray, normalsArray, tangentsArray);
	(void)furthest;

	std::vector<GLuint> indicesArray;
	for (int i = 0; i < (int) indices.size(); i++) {
		GLuint u = indices[i];
		indicesArray.push_back(u);
	}

	// free allocated Vertex objects
	for (int i = 0; i < (int) vertices.size(); i++) {
		delete vertices[i];
	}

	return loader.loadToVAO(verticesArray, texturesArray, normalsArray,
				tangentsArray, indicesArray);
}

void NormalMappingOBJLoader::calculateTangents(
	VertexNM* v0, VertexNM* v1, VertexNM* v2,
	std::vector<glm::vec2>& textures)
{
	glm::vec3 deltaPos1 = v1->getPosition() - v0->getPosition();
	glm::vec3 deltaPos2 = v2->getPosition() - v0->getPosition();
	glm::vec2 uv0 = textures[v0->getTextureIndex()];
	glm::vec2 uv1 = textures[v1->getTextureIndex()];
	glm::vec2 uv2 = textures[v2->getTextureIndex()];
	glm::vec2 deltaUv1 = uv1 - uv0;
	glm::vec2 deltaUv2 = uv2 - uv0;

	GLfloat r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
	deltaPos1 *= deltaUv2.y;
	deltaPos2 *= deltaUv1.y;
	glm::vec3 tangent = deltaPos1 - deltaPos2;
	tangent *= r;
	v0->addTangent(tangent);
	v1->addTangent(tangent);
	v2->addTangent(tangent);
}

VertexNM* NormalMappingOBJLoader::processVertex(
	int index,
	int textureIndex,
	int normalIndex,
	std::vector<VertexNM*>& vertices,
	std::vector<GLuint>& indices)
{
	VertexNM* currentVertex = vertices[index];
	if (!currentVertex->isSet()) {
		currentVertex->setTextureIndex(textureIndex);
		currentVertex->setNormalIndex(normalIndex);
		indices.push_back(index);
		return currentVertex;
	}
	else {
		return dealWithAlreadyProcessedVertex(
			currentVertex, textureIndex, normalIndex, indices, vertices);
	}
}

GLfloat NormalMappingOBJLoader::convertDataToArrays(
	std::vector<VertexNM*>& vertices,
	std::vector<glm::vec2>& textures,
	std::vector<glm::vec3>& normals,
	std::vector<GLfloat>& verticesArray,
	std::vector<GLfloat>& texturesArray,
	std::vector<GLfloat>& normalsArray,
	std::vector<GLfloat>& tangentsArray)
{
	GLfloat furthestPoint = 0;

	for (int i = 0; i < (int) vertices.size(); i++) {
		VertexNM* currentVertex = vertices[i];
		if (currentVertex->getLength() > furthestPoint) {
			furthestPoint = currentVertex->getLength();
		}
		glm::vec3 position = currentVertex->getPosition();
		glm::vec2 textureCoord = textures[currentVertex->getTextureIndex()];
		glm::vec3 normalVector = normals[currentVertex->getNormalIndex()];
		glm::vec3 tangent = currentVertex->getAverageTangent();

		verticesArray.push_back(position.x);
		verticesArray.push_back(position.y);
		verticesArray.push_back(position.z);
		texturesArray.push_back(textureCoord.x);
		texturesArray.push_back(1.0 - textureCoord.y);
		normalsArray.push_back(normalVector.x);
		normalsArray.push_back(normalVector.y);
		normalsArray.push_back(normalVector.z);
		tangentsArray.push_back(tangent.x);
		tangentsArray.push_back(tangent.y);
		tangentsArray.push_back(tangent.z);
	}
	return furthestPoint;
}

VertexNM* NormalMappingOBJLoader::dealWithAlreadyProcessedVertex(
	VertexNM *previousVertex,
	int newTextureIndex,
	int newNormalIndex,
	std::vector<GLuint>& indices,
	std::vector<VertexNM*>& vertices)
{
	if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
		indices.push_back(previousVertex->getIndex());
		return previousVertex;
	} else {
		VertexNM* anotherVertex = previousVertex->getDuplicateVertex();
		if (anotherVertex != nullptr) {
			return dealWithAlreadyProcessedVertex(anotherVertex, newTextureIndex, newNormalIndex,
				indices, vertices);
		} else {
			VertexNM* duplicateVertex = new VertexNM(vertices.size(), previousVertex->getPosition());
			duplicateVertex->setTextureIndex(newTextureIndex);
			duplicateVertex->setNormalIndex(newNormalIndex);
			previousVertex->setDuplicateVertex(duplicateVertex);
			vertices.push_back(duplicateVertex);
			indices.push_back(duplicateVertex->getIndex());
			return duplicateVertex;
		}
	}
}

void NormalMappingOBJLoader::removeUnusedVertices(std::vector<VertexNM*>& vertices)
{
	/*
	vector<Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		Vertex* vertex = *it;
		if (!vertex->isSet()) {
			vertex->setTextureIndex(0);
			vertex->setNormalIndex(0);
		}
	}
	*/
	for (VertexNM* vertex : vertices) {
		// TODO: should be done in an own function
		vertex->averageTangents();
		if (!vertex->isSet()) {
			vertex->setTextureIndex(0);
			vertex->setNormalIndex(0);
		}
	}
}