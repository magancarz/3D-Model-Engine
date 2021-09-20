#include "Renderer.h"

Renderer::Renderer(StaticShader& shader) {
	createProjectionMatrix();
	shader.start();
	shader.loadProjectionMatrix(m_projectionMatrix);
	shader.stop();
}

void Renderer::prepare() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 0, 0, 1);
}

void Renderer::render(const Entity& entity, StaticShader& shader) {
	TexturedModel texturedModel = entity.getTexturedModel();
	RawModel model = texturedModel.getRawModel();
	glBindVertexArray(model.getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glm::mat4 transformationMatrix = createTransformationMatrix(entity.getPosition(), entity.getRotX(), entity.getRotY(), entity.getRotZ(), entity.getScale());
	shader.loadTransformationMatrix(transformationMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturedModel.getTexture().getID());
	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Renderer::createProjectionMatrix() {
	float aspectRatio = (float)1280 / (float)800;
	m_projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, NEAR_PLANE, FAR_PLANE);
}