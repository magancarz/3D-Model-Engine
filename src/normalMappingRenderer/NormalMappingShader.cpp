#include "NormalMappingShader.h"

#include <GL/glew.h>

#include <string>

NormalMappingShader::NormalMappingShader()
	: ShaderProgram("res/shaders/normalMapVert.glsl", "res/shaders/normalMapFrag.glsl") {
	bindAttributes();
	getAllUniformLocations();
}

void NormalMappingShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
	bindAttribute(3, "tangent");
}

void NormalMappingShader::getAllUniformLocations()
{
	location_transformationMatrix = getUniformLocation("transformationMatrix");
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_shineDamper = getUniformLocation("shineDamper");
	location_reflectivity = getUniformLocation("reflectivity");
	location_useFakeLighting = getUniformLocation("useFakeLighting");
	location_skyColor = getUniformLocation("skyColor");
	location_fogDensity = getUniformLocation("fogDensity");
	location_fogGradient = getUniformLocation("fogGradient");
	location_numberOfRows = getUniformLocation("numberOfRows");
	location_textureOffset = getUniformLocation("textureOffset");
	location_clipPlane = getUniformLocation("clipPlane");
	location_modelTexture = getUniformLocation("modelTexture");
	location_normalMap = getUniformLocation("normalMap");

	for (int i = 0; i < MAX_LIGHTS; i++) {
		std::string iStr = std::to_string(i);
		location_lightPosition[i] = getUniformLocation("lightPosition[" + iStr + "]");
		location_lightColor[i] = getUniformLocation("lightColor[" + iStr + "]");
		location_attenuation[i] = getUniformLocation("attenuation[" + iStr + "]");
	}
}

void NormalMappingShader::connectTextureUnits()
{
	loadInt(location_modelTexture, 0);
	loadInt(location_normalMap, 1);
}

void NormalMappingShader::loadTransformationMatrix(glm::mat4 matrix)
{
	loadMatrix(location_transformationMatrix, matrix);
}

void NormalMappingShader::loadLights(std::vector<Light*>& lights, glm::mat4 viewMatrix)
{
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < (int)lights.size()) {
			Light* light = lights[i];
			loadVector3f(location_lightPosition[i], light->getPosition());
			loadVector3f(location_lightColor[i], light->getColor());
			loadVector3f(location_attenuation[i], light->getAttenuation());
		} else {
			// If less than MAX_LIGHTS lights are in the lights vector,
			// load up empty information to the shaders.
			glm::vec3 zero(0.0f, 0.0f, 0.0f);
			glm::vec3 unit(1.0f, 0.0f, 0.0f);
			loadVector3f(location_lightPosition[i], zero);
			loadVector3f(location_lightColor[i], zero);
			loadVector3f(location_attenuation[i], unit);
		}
	}
}

void NormalMappingShader::loadProjectionMatrix(glm::mat4 matrix)
{
	loadMatrix(location_projectionMatrix, matrix);
}

void NormalMappingShader::loadViewMatrix(Camera& camera)
{
	glm::mat4 viewMatrix = camera.getView();
	loadMatrix(location_viewMatrix, viewMatrix);
}

/*
void NormalMappingShader::loadFakeLightingVariable(bool useFakeLighting)
{
	loadBoolean(location_useFakeLighting, useFakeLighting);
}
*/

void NormalMappingShader::loadShineVariables(GLfloat damper, GLfloat reflectivity)
{
	loadFloat(location_shineDamper, damper);
	loadFloat(location_reflectivity, reflectivity);
}

void NormalMappingShader::loadSkyColor(GLfloat r, GLfloat g, GLfloat b)
{
	glm::vec3 vec(r, g, b);
	loadVector3f(location_skyColor, vec);
}

/*
void NormalMappingShader::loadFogVariables(GLfloat density, GLfloat gradient)
{
	loadFloat(location_fogDensity, density);
	loadFloat(location_fogGradient, gradient);
}
*/

void NormalMappingShader::loadNumberOfRows(int numberOfRows)
{
	loadFloat(location_numberOfRows, (GLfloat)numberOfRows);
}

void NormalMappingShader::loadOffset(GLfloat x, GLfloat y)
{
	glm::vec2 vec(x, y);
	loadVector2f(location_textureOffset, vec);
}

void NormalMappingShader::loadClipPlane(glm::vec4 vec)
{
	loadVector4f(location_clipPlane, vec);
}