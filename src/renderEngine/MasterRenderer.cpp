#include "MasterRenderer.h"

MasterRenderer::MasterRenderer() {
	m_entities = new std::map<TexturedModel*, std::vector<Entity*>*>;

	createProjectionMatrix();
}

MasterRenderer::~MasterRenderer() {
	m_shader.cleanUp();

	delete m_entities;
}

void MasterRenderer::render(Light& sun, Camera& camera) {
	m_renderer.prepare();
	m_shader.start();
	m_shader.loadLight(sun);
	m_shader.loadViewMatrix(camera);
	m_shader.loadProjectionMatrix(m_projectionMatrix);
	m_renderer.render(m_entities);
	m_shader.stop();
	m_entities->clear();
}

void MasterRenderer::processEntity(Entity& entity) {
	TexturedModel& entityModel = entity.getTexturedModel();
	std::map<TexturedModel*, std::vector<Entity*>*>::iterator it;

	it = m_entities->find(&entityModel);
	if(it != m_entities->end()) {
		std::vector<Entity*>* batch = it->second;
		batch->push_back(&entity);
	} else {
		std::vector<Entity*>* newBatch = new std::vector<Entity*>();
		newBatch->push_back(&entity);
		m_entities->insert(std::make_pair(&entityModel, newBatch));
	}
}

void MasterRenderer::createProjectionMatrix() {
	m_projectionMatrix = glm::perspective(glm::radians(70.0f), (float)(WINDOW_WIDTH / WINDOW_HEIGHT), 1.0f, 1000.0f);
}