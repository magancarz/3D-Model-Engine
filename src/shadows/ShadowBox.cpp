#include "ShadowBox.h"

#include "toolbox/DisplayManager.h"
#include "../renderEngine/MasterRenderer.h"

ShadowBox::ShadowBox(glm::mat4* lightViewMatrix, Camera* camera):
m_lightViewMatrix(lightViewMatrix), m_cam(camera) {
	calculateWidthsAndHeights();
}

void ShadowBox::update() {
	glm::mat4 rotation = calculateCameraRotationMatrix();
	glm::vec3 forwardVector = rotation * FORWARD;

	glm::vec3 toFar = glm::vec3(forwardVector);
	toFar *= SHADOW_DISTANCE;
	glm::vec3 toNear = glm::vec3(forwardVector);
	toNear *= NEAR_PLANE;
	glm::vec3 centerNear = toNear + m_cam->getPosition();
	glm::vec3 centerFar = toFar + m_cam->getPosition();

	std::vector<glm::vec4> points = calculateFrustumVertices(rotation, forwardVector, centerNear, centerFar);

	bool first = true;
	for(std::vector<glm::vec4>::iterator it = points.begin(); it != points.end(); it++) {
		glm::vec4 point = *it;
		if(first) {
			m_minX = point.x;
			m_maxX = point.x;
			m_minY = point.y;
			m_maxY = point.y;
			m_minZ = point.z;
			m_maxZ = point.z;
			first = false;
			continue;
		}

		if(point.x > m_maxX) {
			m_maxX = point.x;
		} else if(point.x < m_minX) {
			m_minX = point.x;
		}

		if(point.y > m_maxY) {
			m_maxY = point.y;
		} else if(point.y < m_minY) {
			m_minY = point.y;
		}

		if(point.z > m_maxZ) {
			m_maxZ = point.z;
		} else if(point.z < m_minZ) {
			m_minZ = point.z;
		}
	}
	m_maxZ += OFFSET;
}

glm::vec3 ShadowBox::getCenter() {
	float x = (m_minX + m_maxX) / 2.f;
	float y = (m_minY + m_maxY) / 2.f;
	float z = (m_minZ + m_maxZ) / 2.f;
	glm::vec4 cen = glm::vec4(x, y, z, 1.f);
	glm::mat4 invertedLight;
	invertedLight = glm::inverse(*m_lightViewMatrix);
	return glm::vec3(invertedLight * cen);
}

std::vector<glm::vec4> ShadowBox::calculateFrustumVertices(const glm::mat4& rotation, const glm::vec3& forwardVector, const glm::vec3& centerNear, const glm::vec3& centerFar) {
	glm::vec3 upVector = rotation * UP;
	glm::vec3 rightVector = glm::cross(forwardVector, upVector);
	glm::vec3 downVector = glm::vec3(-upVector.x, -upVector.y, -upVector.z);
	glm::vec3 leftVector = glm::vec3(-rightVector.x, -rightVector.y, -rightVector.z);
	glm::vec3 farTop = centerFar + glm::vec3(upVector.x * m_farHeight, upVector.y * m_farHeight, upVector.z * m_farHeight);
	glm::vec3 farBottom = centerFar + glm::vec3(downVector.x * m_farHeight, downVector.y * m_farHeight, downVector.z * m_farHeight);
	glm::vec3 nearTop = centerNear + glm::vec3(upVector.x * m_nearHeight, upVector.y * m_nearHeight, upVector.z * m_nearHeight);
	glm::vec3 nearBottom = centerNear + glm::vec3(downVector.x * m_nearHeight, downVector.y * m_nearHeight, downVector.z * m_nearHeight);
	std::vector<glm::vec4> points;
	points.push_back(calculateLightSpaceFrustumCorner(farTop, rightVector, m_farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(farTop, leftVector, m_farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(farBottom, rightVector, m_farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(farBottom, leftVector, m_farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearTop, rightVector, m_nearWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearTop, leftVector, m_nearWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearBottom, rightVector, m_nearWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearBottom, leftVector, m_nearWidth));

	return points;
}

glm::vec4 ShadowBox::calculateLightSpaceFrustumCorner(glm::vec3& startPoint, glm::vec3& direction, float width) {
	glm::vec3 point = startPoint + glm::vec3(direction.x * width, direction.y * width, direction.z * width);
	glm::vec4 point4f = glm::vec4((*m_lightViewMatrix) * glm::vec4(point.x, point.y, point.z, 1.f));
	return point4f;
}

glm::mat4 ShadowBox::calculateCameraRotationMatrix() {
	glm::mat4 rotation;
	rotation = glm::rotate(rotation, glm::radians(-m_cam->getYaw()), glm::vec3(0, 1, 0));
	rotation = glm::rotate(rotation, glm::radians(-m_cam->getPitch()), glm::vec3(1, 0, 0));
	return rotation;
}

void ShadowBox::calculateWidthsAndHeights() {
	m_farWidth = (float) (SHADOW_DISTANCE * glm::tan(glm::radians(FOV)));
	m_nearWidth = (float) (NEAR_PLANE * glm::tan(glm::radians(FOV)));
	m_farHeight = m_farWidth / getAspectRatio();
	m_nearHeight = m_nearWidth / getAspectRatio();
}

float ShadowBox::getAspectRatio() {
	return (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
}