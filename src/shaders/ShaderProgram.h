#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
public:
	ShaderProgram(const std::string& vertexFile, const std::string& fragmentFile);
	
	void start();
	void stop();
	void cleanUp();
protected:
	void virtual bindAttributes() = 0;
	void virtual getAllUniformLocations() = 0;

	void bindAttribute(const unsigned int attribute, const char* variableName);

	int getUniformLocation(const std::string& uniformName);
	void loadFloat(unsigned int location, float value);
	void loadBoolean(unsigned int location, bool value);
	void loadVector3f(unsigned int location, glm::vec3 vector);
	void loadMatrix(unsigned int location, glm::mat4 matrix);
private:
	unsigned int m_programID;
	unsigned int m_vertexShaderID;
	unsigned int m_fragmentShaderID;

	unsigned int loadShader(const std::string& file, const unsigned int type);
};