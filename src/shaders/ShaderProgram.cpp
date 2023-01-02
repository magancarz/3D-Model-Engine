#include "ShaderProgram.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertex_file, const std::string& fragment_file) {
	m_vertex_shader_id = load_shader(vertex_file, GL_VERTEX_SHADER);
	m_fragment_shader_id = load_shader(fragment_file, GL_FRAGMENT_SHADER);
	m_program_id = glCreateProgram();
	glAttachShader(m_program_id, m_vertex_shader_id);
	glAttachShader(m_program_id, m_fragment_shader_id);
	glLinkProgram(m_program_id);
	glValidateProgram(m_program_id);
}

void ShaderProgram::start() const {
	glUseProgram(m_program_id);
}

void ShaderProgram::stop() {
	glUseProgram(0);
}

void ShaderProgram::clean_up() const {
	glDetachShader(m_program_id, m_vertex_shader_id);
	glDetachShader(m_program_id, m_fragment_shader_id);
	glDeleteShader(m_vertex_shader_id);
	glDeleteShader(m_fragment_shader_id);
	glDeleteProgram(m_program_id);
}

void ShaderProgram::bind_attribute(const unsigned int attribute, const char* variable_name) const {
	glBindAttribLocation(m_program_id, attribute, variable_name);
}

int ShaderProgram::get_uniform_location(const std::string& uniform_name) const {
	return glGetUniformLocation(m_program_id, uniform_name.c_str());
}

void ShaderProgram::load_int(const int location, const int value) {
	glUniform1i(location, value);
}

void ShaderProgram::load_float(const int location, const float value) {
	glUniform1f(location, value);
}

void ShaderProgram::load_boolean(const int location, const bool value) {
	glUniform1i(location, value ? 1 : 0);
}

void ShaderProgram::load_vector2_f(const int location, const glm::vec2& vector) {
	glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::load_vector3_f(const int location, const glm::vec3& vector) {
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::load_vector4_f(const int location, const glm::vec4& vector) {
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::load_matrix(const int location, const glm::mat4& matrix) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int ShaderProgram::load_shader(const std::string& file, const unsigned int type) const {
	std::ifstream stream(file);
	std::string line;
	std::stringstream ss;

	while(getline(stream, line)) {
		ss << line << '\n';
	}
	std::string shader = ss.str();

	const char* src = shader.c_str();
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		auto message = static_cast<char*>(alloca(length * sizeof(char)));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}