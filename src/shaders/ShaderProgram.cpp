#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vertexFile, const std::string& fragmentFile) {
	m_vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
	m_fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);
	glLinkProgram(m_programID);
	glValidateProgram(m_programID);
}

void ShaderProgram::start() {
	glUseProgram(m_programID);
}

void ShaderProgram::stop() {
	glUseProgram(0);
}

void ShaderProgram::cleanUp() {
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
	glDeleteProgram(m_programID);
}

void ShaderProgram::bindAttribute(const unsigned int attribute, const char* variableName) {
	glBindAttribLocation(m_programID, attribute, variableName);
}

int ShaderProgram::getUniformLocation(const std::string& uniformName) {
	return glGetUniformLocation(m_programID, uniformName.c_str());
}

void ShaderProgram::loadInt(unsigned int location, int value) {
	glUniform1i(location, value);
}

void ShaderProgram::loadFloat(unsigned int location, float value) {
	glUniform1f(location, value);
}

void ShaderProgram::loadBoolean(unsigned int location, bool value) {
	glUniform1i(location, value ? 1 : 0);
}

void ShaderProgram::loadVector2f(unsigned int location, glm::vec2 vector) {
	glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::loadVector3f(unsigned int location, glm::vec3 vector) {
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::loadMatrix(unsigned int location, glm::mat4 matrix) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int ShaderProgram::loadShader(const std::string& file, const unsigned int type) {
	std::ifstream stream(file);
	std::string line;
	std::stringstream ss;

	while(getline(stream, line)) {
		ss << line << '\n';
	}
	std::string vertexShader = ss.str();

	const char* src = vertexShader.c_str();
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}