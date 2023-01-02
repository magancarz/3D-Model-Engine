#include "NormalMappingShader.h"

#include <GL/glew.h>

#include <string>

NormalMappingShader::NormalMappingShader()
	: ShaderProgram("res/shaders/normalMapVert.glsl", "res/shaders/normalMapFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void NormalMappingShader::bind_attributes()
{
	bind_attribute(0, "position");
	bind_attribute(1, "textureCoords");
	bind_attribute(2, "normal");
	bind_attribute(3, "tangent");
}

void NormalMappingShader::get_all_uniform_locations()
{
	location_transformationMatrix = get_uniform_location("transformationMatrix");
	location_projectionMatrix = get_uniform_location("projectionMatrix");
	location_viewMatrix = get_uniform_location("viewMatrix");
	location_shineDamper = get_uniform_location("shineDamper");
	location_reflectivity = get_uniform_location("reflectivity");
	location_useFakeLighting = get_uniform_location("useFakeLighting");
	location_skyColor = get_uniform_location("skyColor");
	location_fogDensity = get_uniform_location("fogDensity");
	location_fogGradient = get_uniform_location("fogGradient");
	location_numberOfRows = get_uniform_location("numberOfRows");
	location_textureOffset = get_uniform_location("textureOffset");
	location_clipPlane = get_uniform_location("clipPlane");
	location_modelTexture = get_uniform_location("modelTexture");
	location_normalMap = get_uniform_location("normalMap");

	for (int i = 0; i < MAX_LIGHTS; i++) {
		std::string iStr = std::to_string(i);
		location_lightPosition[i] = get_uniform_location("lightPosition[" + iStr + "]");
		location_lightColor[i] = get_uniform_location("lightColor[" + iStr + "]");
		location_attenuation[i] = get_uniform_location("attenuation[" + iStr + "]");
	}
}

void NormalMappingShader::connectTextureUnits()
{
	load_int(location_modelTexture, 0);
	load_int(location_normalMap, 1);
}

void NormalMappingShader::loadTransformationMatrix(glm::mat4 matrix)
{
	load_matrix(location_transformationMatrix, matrix);
}

void NormalMappingShader::loadLights(std::vector<Light*>& lights, glm::mat4 viewMatrix)
{
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < (int)lights.size()) {
			Light* light = lights[i];
			load_vector3_f(location_lightPosition[i], light->get_position());
			load_vector3_f(location_lightColor[i], light->get_color());
			load_vector3_f(location_attenuation[i], light->get_attenuation());
		} else {
			// If less than MAX_LIGHTS lights are in the lights vector,
			// load up empty information to the shaders.
			glm::vec3 zero(0.0f, 0.0f, 0.0f);
			glm::vec3 unit(1.0f, 0.0f, 0.0f);
			load_vector3_f(location_lightPosition[i], zero);
			load_vector3_f(location_lightColor[i], zero);
			load_vector3_f(location_attenuation[i], unit);
		}
	}
}

void NormalMappingShader::loadProjectionMatrix(glm::mat4 matrix)
{
	load_matrix(location_projectionMatrix, matrix);
}

void NormalMappingShader::loadViewMatrix(Camera& camera)
{
	glm::mat4 viewMatrix = camera.getView();
	load_matrix(location_viewMatrix, viewMatrix);
}

/*
void NormalMappingShader::loadFakeLightingVariable(bool useFakeLighting)
{
	load_boolean(location_useFakeLighting, useFakeLighting);
}
*/

void NormalMappingShader::loadShineVariables(GLfloat damper, GLfloat reflectivity)
{
	load_float(location_shineDamper, damper);
	load_float(location_reflectivity, reflectivity);
}

void NormalMappingShader::loadSkyColor(GLfloat r, GLfloat g, GLfloat b)
{
	glm::vec3 vec(r, g, b);
	load_vector3_f(location_skyColor, vec);
}

/*
void NormalMappingShader::loadFogVariables(GLfloat density, GLfloat gradient)
{
	load_float(location_fogDensity, density);
	load_float(location_fogGradient, gradient);
}
*/

void NormalMappingShader::loadNumberOfRows(int numberOfRows)
{
	load_float(location_numberOfRows, (GLfloat)numberOfRows);
}

void NormalMappingShader::loadOffset(GLfloat x, GLfloat y)
{
	glm::vec2 vec(x, y);
	load_vector2_f(location_textureOffset, vec);
}

void NormalMappingShader::loadClipPlane(glm::vec4 vec)
{
	load_vector4_f(location_clipPlane, vec);
}