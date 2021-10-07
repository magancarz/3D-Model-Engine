#include "OBJLoader.h"

void dealWithAlreadyProcessedVertex(Vertex* previousVertex, int newTextureIndex, int newNormalIndex,
	std::vector<unsigned int>& indices, std::vector<Vertex*>& vertices) {
	if(previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
		indices.push_back(previousVertex->getIndex());
	} else {
		Vertex* anotherVertex = previousVertex->getDuplicateVertex();
		if(anotherVertex != nullptr) {
			dealWithAlreadyProcessedVertex(anotherVertex, newTextureIndex, newNormalIndex, indices, vertices);
		} else {
			Vertex* duplicateVertex = new Vertex(vertices.size(), previousVertex->getPosition());
			duplicateVertex->setTextureIndex(newTextureIndex);
			duplicateVertex->setNormalIndex(newNormalIndex);
			previousVertex->setDuplicateVertex(duplicateVertex);
			vertices.push_back(duplicateVertex);
			indices.push_back(duplicateVertex->getIndex());
		}
	}
}

void processVertex(int index, int textureIndex, int normalIndex, std::vector<Vertex*>& vertices, std::vector<unsigned int>& indices) {
	Vertex* currentVertex = vertices[index];
	if(!currentVertex->isSet()) {
		currentVertex->setTextureIndex(textureIndex);
		currentVertex->setNormalIndex(normalIndex);
		indices.push_back(index);
	} else {
		dealWithAlreadyProcessedVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
	}
}

float convertDataToArrays(std::vector<Vertex*>& vertices, std::vector<glm::vec2>& textures,
	std::vector<glm::vec3>& normals, std::vector<float>& verticesArray,
	std::vector<float>& texturesArray, std::vector<float>& normalsArray) {
	float furthestPoint = 0;

	for(size_t i = 0; i < vertices.size(); i++) {
		Vertex* currentVertex = vertices[i];
		if(currentVertex->getLength() > furthestPoint)
			furthestPoint = currentVertex->getLength();

		glm::vec3 position = currentVertex->getPosition();
		glm::vec2 textureCoord = textures[currentVertex->getTextureIndex()];
		glm::vec3 normalVector = normals[currentVertex->getNormalIndex()];

		verticesArray.push_back(position[0]);
		verticesArray.push_back(position[1]);
		verticesArray.push_back(position[2]);
		texturesArray.push_back(textureCoord[0]);
		texturesArray.push_back(1.0f - textureCoord[1]);
		normalsArray.push_back(normalVector[0]);
		normalsArray.push_back(normalVector[1]);
		normalsArray.push_back(normalVector[2]);
	}

	return furthestPoint;
}

void removeUnusedVertices(std::vector<Vertex*>& vertices) {
	std::vector<Vertex*>::iterator it;

	for(it = vertices.begin(); it != vertices.end(); it++) {
		Vertex* vertex = *it;
		if(!vertex->isSet()) {
			vertex->setTextureIndex(0);
			vertex->setNormalIndex(0);
		}
	}
}

RawModel* loadOBJ(const std::string& fileName, Loader& loader) {
	std::ifstream inFile(fileName, std::ios::in);

	if(!inFile) {
		std::cerr << "Unable to load obj file!\n";
		exit(1);
	}

	std::string line;
	std::vector<Vertex*> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;

	//This loop collects the vertices, texture coords and normals from the obj file.
	while(!inFile.eof()) {
		getline(inFile, line);

		std::istringstream iss(line);
		std::string starts;
		GLfloat x, y, z;

		//Starts contains e.g. v, vt, tv, s, f
		iss >> starts;

		if(starts == "v") {
			//e.g. v 3.227124 -0.065127 -1.000000
			iss >> x >> y >> z;
			glm::vec3 vertex(x, y, z);
			Vertex* newVertex = new Vertex(vertices.size(), vertex);
			vertices.push_back(newVertex);
		} else if(starts == "vt") {
			//e.g. vt 0.905299 0.932320
			iss >> x >> y;
			glm::vec2 texture(x, y);
			textures.push_back(texture);
		} else if(starts == "vn") {
			//e.g. vn -1.000000 0.000000 0.000000
			iss >> x >> y >> z;
			glm::vec3 normal(x, y, z);
			normals.push_back(normal);
		} else if(starts == "f") {
			//break when faces start
			break;
		}
	}

	std::vector<GLfloat> verticesArray;
	std::vector<GLfloat> normalsArray;
	std::vector<GLfloat> textureArray;
	std::vector<GLuint> indicesArray;

	//read the faces in a second loop
	while(!inFile.eof()) {
		if(line == "")
			break;

		std::istringstream iss(line);
		std::string starts;
		GLuint u[9];

		iss >> starts;

		if(starts == "f") {
			//e.g. f 41/1/1 38/2/1 45/3/1
			std::string tmp, f = "";
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp + " ";
			iss >> tmp;
			f += tmp;

			//replace /'s with space
			size_t x = f.find("/");
			while(x < std::string::npos) {
				f.replace(x, 1, " ");
				x = f.find("/", x + 1);
			}
			std::istringstream iss2(f);
			for(int i = 0; i < 9; i++) {
				iss2 >> u[i];
				//the indices in the obj file start from 1, ours start from 0
				u[i]--;
			}

			//process vertices
			processVertex(u[0], u[1], u[2], vertices, indices);
			processVertex(u[3], u[4], u[5], vertices, indices);
			processVertex(u[6], u[7], u[8], vertices, indices);
		}

		getline(inFile, line);
	}
	
	removeUnusedVertices(vertices);
	
	float furthest = convertDataToArrays(vertices, textures, normals, verticesArray, textureArray, normalsArray);

	for(int i = 0; i < (int)indices.size(); i++) {
		unsigned int u = indices[i];
		indicesArray.push_back(u);
	}

	for(int i = 0; i < (int)vertices.size(); i++)
		delete vertices[i];

	return loader.loadToVAO(verticesArray, textureArray, normalsArray, indicesArray);

}