#pragma once

#include "../entities/Camera.h"

class ShadowBox {
public:
	/**
	Creates a new shadow box and calculates some initial values relating to
	the camera's view frustum, namely the width and height of the near plane
	and (possibly adjusted) far plane.
	*/
	ShadowBox(glm::mat4* lightViewMatrix, Camera* camera);

	/**
	Updates the bounds of the shadow box based on the light direction and the
	camera's view frustum, to make sure that the box covers the smallest area
	possible while still ensuring that everything inside the camera's view
	(within a ceratin range) will cast shadows.
	*/
	void update();

	/**
	Calculates the center of the "view cuboid" in light space first, and then
	converts this to world space using the inverse light's view matrix.
	*/
	glm::vec3 getCenter();

	void setLightViewMatrix(glm::mat4* lightViewMatrix) { m_lightViewMatrix = lightViewMatrix; }

	float getWidth() { return m_maxX - m_minX; }
	float getHeight() { return m_maxY - m_minY; }
	float getLength() { return m_maxZ - m_minZ; }

private:
	/**
	Calculates the position of the vertex at each corner of the view frustum
	in light space (8 vertices in total, so this returns 8 positions).
	*/
	std::vector<glm::vec4> calculateFrustumVertices(const glm::mat4& rotation, const glm::vec3& forwardVector, const glm::vec3& centerNear, const glm::vec3& centerFar);

	/**
	Calculates one of the corner vertices of the view frustum in world space
	and converts it to light space.
	*/
	glm::vec4 calculateLightSpaceFrustumCorner(glm::vec3& startPoint, glm::vec3& direction, float width);

	/**
	The rotation of the camera represented as a matrix.
	*/
	glm::mat4 calculateCameraRotationMatrix();

	/**
	Calculates the width and height of the near and far planes of the
	camera's view frustum. However, this doesn't have to use the "actual" far
	plane of the view frustum. It can use a shortened view frustum if desired
	by bringing the far-plane closer, which would increase shadow resolution
	but means that distant objects wouldn't cast shadows.
	*/
	void calculateWidthsAndHeights();

	/**
	The aspect ratio of the display (width:height ratio).
	*/
	float getAspectRatio();

	const float OFFSET = 10.0f,
				SHADOW_DISTANCE = 100.0f;

	const glm::vec4 UP = glm::vec4(0, 1, 0, 0),
					FORWARD = glm::vec4(0, 0, -1, 0);

	float m_minX = 0, m_maxX = 0,
		  m_minY = 0, m_maxY = 0,
		  m_minZ = 0, m_maxZ = 0;

	glm::mat4* m_lightViewMatrix;
	Camera* m_cam;

	float m_farHeight, m_farWidth, m_nearHeight, m_nearWidth;
};