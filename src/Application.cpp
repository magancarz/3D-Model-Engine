#include "Headers.h"

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/MasterRenderer.h"
#include "renderEngine/OBJLoader.h"
#include "normalMappingRenderer/NormalMappingOBJLoader.h"
#include "models/RawModel.h"
#include "models/TexturedModel.h"
#include "entities/Camera.h"
#include "entities/Player.h"
#include "guis/GuiRenderer.h"
#include "water/WaterRenderer.h"
#include "water/WaterFrameBuffers.h"
#include "renderEngine/FBO.h"
#include "renderEngine/PostProcessing.h"

int main(void) {
	bool POST_PROCESSING_ENABLED = true;

    /* initialize the glfw library */
    if(!glfwInit())
        return -1;

    DisplayManager::createDisplay();
    Input::initialize_input();

    /* check OpenGL version */
    std::cout << "OpenGL version supported by this platform: " << glGetString(GL_VERSION) << std::endl;

    /* initialize model loaders */
    auto loader = std::make_unique<Loader>();
    auto normal_mapped_obj_loader = std::make_unique<NormalMappingOBJLoader>();

    /* create terrain */
    auto backgroundTexture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/grass.png"));
    auto rTexture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/mud.png"));
    auto gTexture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/grassFlowers.png"));
    auto bTexture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/path.png"));

    auto texturePack = std::make_unique<TerrainTexturePack>(*backgroundTexture, *rTexture, *gTexture, *bTexture);
    auto blendMap = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/black.png"));

    auto terrain = std::make_unique<Terrain>(0, 0, loader.get(), texturePack.get(), blendMap.get());

    /* load 3d models */
    //=====OBJECTS=====///
    auto stallModel = std::unique_ptr<RawModel>(loadOBJ("res/models/stall.obj", loader.get()));
    ModelTexture stallTexture(loader->loadTexture("res/textures/stallTexture.png"));
    TexturedModel texturedStallModel(*stallModel, stallTexture);

    auto cherryTreeModel = loadOBJ("res/models/cherry.obj", loader.get());
    ModelTexture cherryTreeTexture(loader->loadTexture("res/textures/cherry.png"));
    TexturedModel texturedCherryTreeModel(*cherryTreeModel, cherryTreeTexture);

    auto lanternModel = loadOBJ("res/models/lantern.obj", loader.get());
    ModelTexture lanternTexture(loader->loadTexture("res/textures/lantern.png"));
    TexturedModel texturedLanternModel(*lanternModel, lanternTexture);
    texturedLanternModel.getTexture().setSpecularMap(loader->loadTexture("res/textures/lanternS.png"));
    
    /* create light objects */
    auto sun = std::make_unique<Light>(glm::vec3(0, 20000, 0), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1, 0.1f, 0.01f));
    auto light2 = std::make_unique<Light>(glm::vec3(278, 12, 224), glm::vec3(0.65,0.65,0.65), glm::vec3(1, 0.01f, 0.002f));
    auto light3 = std::make_unique<Light>(glm::vec3(20, 0, 10), glm::vec3(0,1,0), glm::vec3(1, 0.01f, 0.002f));
    auto light4 = std::make_unique<Light>(glm::vec3(30, 0, 10), glm::vec3(0,0,1), glm::vec3(1, 0.01f, 0.002f));

    std::vector<Light*> lights;
    lights.push_back(sun.get());
    lights.push_back(light2.get());
    lights.push_back(light3.get());
    lights.push_back(light4.get());

    /* create player and camera */
    auto player = std::make_unique<Player>(texturedStallModel, glm::vec3(280, 4.5f, 208), 0, 0, 0, 0.000001f);

    //Camera
    auto camera = std::make_unique<Camera>(*player, glm::vec3(-5.0f, 6.0f, -5.0f));

    /* create renderers */
    MasterRenderer renderer(loader.get(), camera.get());

    GuiRenderer guiRenderer(loader.get());

    /* create water renderer setup */
    auto waterShader = std::make_unique<WaterShader>();
    auto fbos = std::make_unique<WaterFrameBuffers>();
    auto waterRenderer = std::make_unique<WaterRenderer>(loader.get(), waterShader.get(), renderer.getProjectionMatrix(), fbos.get());
    std::vector<WaterTile*> waters;
    auto water = std::make_unique<WaterTile>(400, 400, 0);
    waters.push_back(water.get());

    /* create entities */
    auto entities = std::make_unique<std::vector<Entity*>>();
	entities->push_back(player.get());


    auto lantern = std::make_unique<Entity>(texturedLanternModel, glm::vec3(270, 0, 234), 0, 0, 0, 1);
    entities->push_back(lantern.get());

    for(int i = 0; i < 300; i++) {
        int treeX,
            treeZ;
        do {
            treeX = rand() % TERRAIN_SIZE;
            treeZ = rand() % TERRAIN_SIZE;
        } while(terrain->getHeightOfTerrain(treeX, treeZ) <= 0);
        int treeRotY = rand() % 180;
        Entity* tree = new Entity(texturedCherryTreeModel, glm::vec3(treeX, terrain->getHeightOfTerrain(treeX, treeZ), treeZ), 0.0, treeRotY, 0.0, 5.0);
        entities->push_back(tree);
    }
    
    /* post-processing effects */
    auto multisampleFBO = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT);
    auto outputFBO1 = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    auto outputFBO2 = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    if(POST_PROCESSING_ENABLED) POST_PROCESSING_INIT(loader.get());

    /* Loop until the user closes the window */
    while(!DisplayManager::isCloseRequested) {
        //Events
        player->move(*terrain, camera->getYaw(), camera->getPitch());
        camera->move();

        //Reset input values
        DisplayManager::resetInputValues();

        /* Poll for and process events */
        glfwPollEvents();

        // before any rendering takes place, render shadow map
        renderer.renderShadowMap(entities.get(), sun.get());

        //OpenGL calls
        glEnable(GL_CLIP_DISTANCE0);

        //Process terrains
        renderer.processTerrain(terrain.get());

        //Process entities
        renderer.processEntities(entities.get());

        //Water
        fbos->bindReflectionFrameBuffer();
        float distance = 2 * (camera->getPosition().y - water->getHeight());
        camera->getPosition().y -= distance;
        camera->invertPitch();
        renderer.render(lights, *camera, glm::vec4(0, 1, 0, -water->getHeight()));
        camera->getPosition().y += distance;
        camera->invertPitch();
        fbos->unbindCurrentFrameBuffer();

        fbos->bindRefractionFrameBuffer();
        renderer.render(lights, *camera, glm::vec4(0, -1, 0, water->getHeight()));

        //Draw here
        glDisable(GL_CLIP_DISTANCE0);
        fbos->unbindCurrentFrameBuffer();

        multisampleFBO->bindFrameBuffer();
        renderer.render(lights, *camera, glm::vec4(0, -1, 0, 10000));

        waterRenderer->render(waters, *camera, *sun);
        
        multisampleFBO->unbindFrameBuffer();
        multisampleFBO->resolveToFBO(GL_COLOR_ATTACHMENT0, outputFBO1.get());
        multisampleFBO->resolveToFBO(GL_COLOR_ATTACHMENT1, outputFBO2.get());
        if(POST_PROCESSING_ENABLED) POST_PROCESSING_DRAW(outputFBO1->getColorTexture(), outputFBO2->getColorTexture());

        //Clean up renderer
        renderer.cleanUp();

        /* Swap front and back buffers */
        DisplayManager::updateDisplay();

        //Check if window needs to close
        DisplayManager::checkCloseRequests();
    }

    //Clean up resources
    loader->cleanUp();
    multisampleFBO->cleanUp();
    if(POST_PROCESSING_ENABLED) POST_PROCESSING_CLEAN_UP();

    DisplayManager::closeDisplay();

    return 0;
}
