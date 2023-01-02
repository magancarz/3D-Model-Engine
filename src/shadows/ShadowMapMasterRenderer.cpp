#include "ShadowMapMasterRenderer.h"

ShadowMapMasterRenderer::ShadowMapMasterRenderer(Camera* camera) {
	m_shader = new ShadowShader();
	m_shadowBox = new ShadowBox(&m_lightViewMatrix, camera);
	m_shadowFBO = new ShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	m_entityRenderer = new ShadowMapEntityRenderer(m_shader, &m_projectionViewMatrix);

	m_offset = createOffset();
}

ShadowMapMasterRenderer::~ShadowMapMasterRenderer() {
	delete m_entityRenderer;
	delete m_shadowFBO;
	delete m_shadowBox;
	delete m_shader;
}

void ShadowMapMasterRenderer::render(const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entitiesMap, const std::shared_ptr<Light>& sun) {
	m_shadowBox->update();
	glm::vec3 sunPosition = sun->get_position();
	glm::vec3 lightDirection = glm::vec3(sunPosition.x, sunPosition.y, sunPosition.z);
	prepare(lightDirection, m_shadowBox);
	m_entityRenderer->render(entitiesMap);
	finish();
}

void ShadowMapMasterRenderer::prepare(glm::vec3 lightDirection, ShadowBox* box) {
	updateOrthoProjectionMatrix(box->getWidth(), box->getHeight(), box->getLength());
	updateLightViewMatrix(lightDirection, box->getCenter());
	m_projectionViewMatrix = m_offset * m_projectionMatrix * m_lightViewMatrix;
	m_shadowFBO->bindFrameBuffer();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shader->start();
}

void ShadowMapMasterRenderer::finish() {
	m_shader->stop();
	m_shadowFBO->unbindFrameBuffer();
}

void ShadowMapMasterRenderer::updateLightViewMatrix(glm::vec3 direction, glm::vec3 center) {
	m_lightViewMatrix = glm::lookAt(direction, glm::vec3(400.f, 0.f, 400.f), glm::vec3(0.f, 1.f, 0.f));
}

void ShadowMapMasterRenderer::updateOrthoProjectionMatrix(float width, float height, float length) {
	/*m_projectionMatrix = glm::mat4(1.0f);
	m_projectionMatrix[0][0] = 2.0f / width;
	m_projectionMatrix[1][1] = 2.0f / height;
	m_projectionMatrix[2][2] = -2.0f / length;
	m_projectionMatrix[3][3] = 1;*/
	float orthoSize = 128;
	m_projectionMatrix = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100000.f);
}

glm::mat4 ShadowMapMasterRenderer::createOffset() {
	glm::mat4 offset = glm::mat4(1.f);
	offset = glm::translate(offset, glm::vec3(0.5f, 0.5f, 0.5f));
	offset = glm::scale(offset, glm::vec3(0.5f, 0.5f, 0.5f));
	return offset;
}