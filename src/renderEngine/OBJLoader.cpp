#include "OBJLoader.h"

void processVertex(int index1, int index2, int index3,
	std::vector<GLuint>& indices, std::vector<glm::vec2>& textures,
	std::vector<glm::vec3>& normals, std::vector<GLfloat>& textureArray,
	std::vector<GLfloat>& normalsArray) {
	GLuint currentVertexPointer = index1;
	indices.push_back(currentVertexPointer);

	glm::vec2 currentTex = textures[index2];
	textureArray[currentVertexPointer * 2] = currentTex.x;
	textureArray[currentVertexPointer * 2 + 1] = 1 - currentTex.y;

	glm::vec3 currentNorm = normals[index3];
	normalsArray[currentVertexPointer * 3] = currentNorm.x;
	normalsArray[currentVertexPointer * 3 + 1] = currentNorm.y;
	normalsArray[currentVertexPointer * 3 + 2] = currentNorm.z;
}

RawModel* loadOBJ(const std::string& fileName, Loader& loader) {
	std::ifstream inFile(fileName, std::ios::in);

	if(!inFile) {
		std::cerr << "Unable to load obj file!\n";
		exit(1);
	}

	std::string line;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;

	std::vector<GLfloat> verticesArray;
	std::vector<GLfloat> normalsArray;
	std::vector<GLfloat> textureArray;
	std::vector<GLuint> indicesArray;

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
			vertices.push_back(vertex);
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
			textureArray.resize(vertices.size() * 2);
			normalsArray.resize(vertices.size() * 3);
			//break when faces start
			break;
		}
	}

	int faces = 0;

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
			processVertex(u[0], u[1], u[2], indices, textures, normals, textureArray, normalsArray);
			processVertex(u[3], u[4], u[5], indices, textures, normals, textureArray, normalsArray);
			processVertex(u[6], u[7], u[8], indices, textures, normals, textureArray, normalsArray);

			faces++;
		}

		getline(inFile, line);
	}
	verticesArray.resize(vertices.size() * 3);
	indicesArray.resize(indices.size());

	int vertexPointer = 0;
	for(auto& vertex : vertices) {
		verticesArray[vertexPointer++] = vertex.x;
		verticesArray[vertexPointer++] = vertex.y;
		verticesArray[vertexPointer++] = vertex.z;
	}

	for(unsigned int i = 0; i < indices.size(); i++) {
		indicesArray[i] = indices[i];
	}

	return loader.loadToVAO(verticesArray, textureArray, normalsArray, indicesArray);

}