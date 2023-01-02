#pragma once

#include <glm/glm.hpp>

#include <string>

class ShaderProgram {
public:
	ShaderProgram(const std::string& vertex_file, const std::string& fragment_file);
	
	void start() const;
	static void stop();
	void clean_up() const;

	void virtual bind_attributes() = 0;
	void virtual get_all_uniform_locations() = 0;

protected:
	void bind_attribute(unsigned int attribute, const char* variable_name) const;

	int get_uniform_location(const std::string& uniform_name) const;

	static void load_int(int location, int value);
	static void load_float(int location, float value);
	static void load_boolean(int location, bool value);
	static void load_vector2_f(int location, const glm::vec2& vector);
	static void load_vector3_f(int location, const glm::vec3& vector);
	static void load_vector4_f(int location, const glm::vec4& vector);
	static void load_matrix(int location, const glm::mat4& matrix);

private:
	unsigned int m_program_id;
	unsigned int m_vertex_shader_id;
	unsigned int m_fragment_shader_id;

	unsigned int load_shader(const std::string& file, unsigned int type) const;
};
