#include "MasterRenderer.h"

void enableCulling() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void disableCulling() {
	glDisable(GL_CULL_FACE);
}

MasterRenderer::MasterRenderer(Loader* loader) {
	m_entities = new std::map<TexturedModel*, std::vector<Entity*>*>;
	m_terrains = new std::vector<Terrain*>;

	m_shader = new StaticShader();
	m_terrainShader = new TerrainShader();

	createProjectionMatrix();

	m_renderer = new EntityRenderer(m_shader, m_projectionMatrix);
	m_terrainRenderer = new TerrainRenderer(m_terrainShader, m_projectionMatrix);
	m_skyboxRenderer = new SkyboxRenderer(loader, m_projectionMatrix);

	m_shader->start();
	m_shader->loadProjectionMatrix(m_projectionMatrix);
	m_shader->stop();

	enableCulling();
}

MasterRenderer::~MasterRenderer() {
	m_terrainShader->cleanUp();
	m_shader->cleanUp();

	delete m_skyboxRenderer;

	delete m_terrainRenderer;
	delete m_terrainShader;
	
	delete m_renderer;
	delete m_shader;

	delete m_terrains;
	delete m_entities;
}

void MasterRenderer::render(std::vector<Light*>& lights, Camera& camera, glm::vec4 clipPlane) {
	prepare();
	m_shader->start();
	m_shader->loadClipPlane(clipPlane);
	m_shader->loadSkyColor(RED, GREEN, BLUE);
	m_shader->loadLights(lights);
	m_shader->loadViewMatrix(camera);
	m_renderer->render(m_entities);
	m_shader->stop();

	m_terrainShader->start();
	m_terrainShader->loadClipPlane(clipPlane);
	m_terrainShader->loadSkyColor(RED, GREEN, BLUE);
	m_terrainShader->loadLights(lights);
	m_terrainShader->loadViewMatrix(camera);
	m_terrainRenderer->render(m_terrains);
	m_terrainShader->stop();

	m_skyboxRenderer->render(camera, RED, GREEN, BLUE);
}

void MasterRenderer::prepare() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(RED, GREEN, BLUE, 1);
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

void MasterRenderer::processTerrain(Terrain* terrain) {
	m_terrains->push_back(terrain);
}

void MasterRenderer::cleanUp() {
	m_entities->clear();
	m_terrains->clear();
}

void MasterRenderer::createProjectionMatrix() {
	m_projectionMatrix = glm::perspective(glm::radians(FOV), (float)(WINDOW_WIDTH / WINDOW_HEIGHT), NEAR_PLANE, FAR_PLANE);
}