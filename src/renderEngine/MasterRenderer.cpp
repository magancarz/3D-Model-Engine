#include "MasterRenderer.h"


MasterRenderer::MasterRenderer(Loader* loader, Camera* camera) {
	m_entities = new std::map<TexturedModel*, std::vector<Entity*>*>;
	m_normalMappingEntities = new std::map<TexturedModel*, std::vector<Entity*>*>;
	m_terrains = new std::vector<Terrain*>;

	m_shader = new StaticShader();
	m_terrainShader = new TerrainShader();

	createProjectionMatrix();

	m_renderer = new EntityRenderer(m_shader, m_projectionMatrix);
	m_terrainRenderer = new TerrainRenderer(m_terrainShader, m_projectionMatrix);
	m_skyboxRenderer = new SkyboxRenderer(loader, m_projectionMatrix);
	m_normalMappingRenderer = new NormalMappingRenderer(m_projectionMatrix);
	m_shadowMapRenderer = new ShadowMapMasterRenderer(camera);

	m_shader->start();
	m_shader->loadProjectionMatrix(m_projectionMatrix);
	m_shader->stop();

}

MasterRenderer::~MasterRenderer() {
	m_terrainShader->cleanUp();
	m_shader->cleanUp();

	delete m_shadowMapRenderer;
	delete m_normalMappingRenderer;
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

	m_normalMappingRenderer->render(m_normalMappingEntities, clipPlane, lights, camera);

	m_terrainShader->start();
	m_terrainShader->loadClipPlane(clipPlane);
	m_terrainShader->loadSkyColor(RED, GREEN, BLUE);
	m_terrainShader->loadLights(lights);
	m_terrainShader->loadViewMatrix(camera);
	m_terrainRenderer->render(m_terrains, m_shadowMapRenderer->getToShadowMapSpaceMatrix());
	m_terrainShader->stop();

	m_skyboxRenderer->render(camera, RED, GREEN, BLUE);
}

void MasterRenderer::renderShadowMap(std::vector<Entity*>* entityList, Light* sun) {
	processEntities(entityList);
	m_shadowMapRenderer->render(m_entities, sun);
	m_entities->clear();
}

void MasterRenderer::prepare() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(RED, GREEN, BLUE, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapRenderer->getShadowMap());
}

void MasterRenderer::enable_culling() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MasterRenderer::disable_culling() {
	glDisable(GL_CULL_FACE);
}

void MasterRenderer::processEntities(std::vector<Entity*>* entityList) {
	for(std::vector<Entity*>::iterator it = entityList->begin(); it != entityList->end(); it++) {
		Entity* entity = *it;
		processEntity(*entity);
	}
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

void MasterRenderer::processNormalMapEntity(Entity& entity) {
	TexturedModel& entityModel = entity.getTexturedModel();
	std::map<TexturedModel*, std::vector<Entity*>*>::iterator it;

	it = m_normalMappingEntities->find(&entityModel);
	if(it != m_normalMappingEntities->end()) {
		std::vector<Entity*>* batch = it->second;
		batch->push_back(&entity);
	} else {
		std::vector<Entity*>* newBatch = new std::vector<Entity*>();
		newBatch->push_back(&entity);
		m_normalMappingEntities->insert(std::make_pair(&entityModel, newBatch));
	}
}

void MasterRenderer::processTerrain(Terrain* terrain) {
	m_terrains->push_back(terrain);
}

void MasterRenderer::cleanUp() {
	m_entities->clear();
	m_normalMappingEntities->clear();
	m_terrains->clear();
}

void MasterRenderer::createProjectionMatrix() {
	m_projectionMatrix = glm::mat4();
	float aspectRatio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
	float yScale = (float) ((1.0f / glm::tan(glm::radians(FOV / 2.0f))));
	float xScale = yScale / aspectRatio;
	float frustumLength = FAR_PLANE - NEAR_PLANE;

	m_projectionMatrix[0][0] = xScale;
	m_projectionMatrix[1][1] = yScale;
	m_projectionMatrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustumLength);
	m_projectionMatrix[2][3] = -1;
	m_projectionMatrix[3][2] = -((2.0f * NEAR_PLANE * FAR_PLANE) / frustumLength);
	m_projectionMatrix[3][3] = 0;
}