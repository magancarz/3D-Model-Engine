#include "Headers.h"

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/MasterRenderer.h"
#include "renderEngine/OBJLoader.h"
#include "normalMappingRenderer/NormalMappingOBJLoader.h"
#include "shaders/StaticShader.h"
#include "models/RawModel.h"
#include "models/TexturedModel.h"
#include "entities/Camera.h"
#include "entities/Player.h"
#include "guis/GuiRenderer.h"
#include "guis/GuiTexture.h"
#include "toolbox/MousePicker.h"
#include "water/WaterRenderer.h"
#include "water/WaterFrameBuffers.h"
#include "fontRendering/TextMaster.h"
#include "particles/ParticleMaster.h"
#include "particles/ParticleSystem.h"
#include "renderEngine/FBO.h"
#include "renderEngine/PostProcessing.h"

//=====GLOBAL VARIABLES=====//
//Main loop control
bool isCloseRequested = false;

//Input manager
Input inputManager;

//Display
DisplayManager display;

//Particle master
ParticleMaster* particleMaster;

//enabled/disabled post-processing
bool POST_PROCESSING_ENABLED = false;

int main(void) {
    /* initialize the glfw library */
    if(!glfwInit())
        return -1;

    /* initialize display */
    display.createDisplay();

    /* check OpenGL version */
    std::cout << "OpenGL version supported by this platform: " << glGetString(GL_VERSION) << std::endl;

    /* initialize model loaders */
    Loader* loader = new Loader;
    NormalMappingOBJLoader* normalMappedLoader = new NormalMappingOBJLoader;

    /* initialize fonts */
    FontType font(loader->loadFontTexture("res/textures/candara.png"), "res/textures/candara.fnt");
    textMaster.init(loader);

    /* create terrain */
    TerrainTexture backgroundTexture(loader->loadTexture("res/textures/grass.png"));
    TerrainTexture rTexture(loader->loadTexture("res/textures/mud.png"));
    TerrainTexture gTexture(loader->loadTexture("res/textures/grassFlowers.png"));
    TerrainTexture bTexture(loader->loadTexture("res/textures/path.png"));

    TerrainTexturePack* texturePack = new TerrainTexturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap(loader->loadTexture("res/textures/blendMap.png"));

    Terrain* terrain = new Terrain(0, 0, loader, texturePack, &blendMap);

    /* load 3d models */
    //=====OBJECTS=====///
    RawModel* stallModel = loadOBJ("res/models/stall.obj", loader);
    ModelTexture stallTexture(loader->loadTexture("res/textures/stallTexture.png"));
    TexturedModel texturedStallModel(*stallModel, stallTexture);
    //ModelTexture& stallTexture = texturedStallModel.getTexture();
    //stallTexture.setShineDamper(10);
    //stallTexture.setReflectivity(5.0f);

    RawModel* treeModel = loadOBJ("res/models/tree.obj", loader);
    ModelTexture treeTexture(loader->loadTexture("res/textures/tree.png"));
    TexturedModel texturedTreeModel(*treeModel, treeTexture);
    //ModelTexture& treeTexture = texturedTreeModel.getTexture();
    //treeTexture.setShineDamper(10);
    //treeTexture.setReflectivity(5.0f);

    //=====NORMAL MAPPED OBJECTS=====//
    RawModel* barrelModel = normalMappedLoader->loadNormalMappedOBJ("res/models/barrel.obj", *loader);
    ModelTexture barrelTexture(loader->loadTexture("res/textures/barrel.png"));
    TexturedModel texturedBarrelModel(*barrelModel, barrelTexture);
    ModelTexture& texture2 = texturedBarrelModel.getTexture();
    texture2.setNormalMap(loader->loadTexture("res/textures/barrelNormal.png"));
    //texture2.setShineDamper(10);
    //texture2.setReflectivity(5.0f);
    
    /* create light objects */
    Light* sun = new Light(glm::vec3(10000, 15000, -10000), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1, 0, 0));
    Light* light2 = new Light(glm::vec3(10, 0, 10), glm::vec3(1,0,0), glm::vec3(1, 0.01f, 0.002f));
    Light* light3 = new Light(glm::vec3(20, 0, 10), glm::vec3(0,1,0), glm::vec3(1, 0.01f, 0.002f));
    Light* light4 = new Light(glm::vec3(30, 0, 10), glm::vec3(0,0,1), glm::vec3(1, 0.01f, 0.002f));

    std::vector<Light*> lights;
    lights.push_back(sun);
    lights.push_back(light2);
    lights.push_back(light3);
    lights.push_back(light4);

    /* create player and camera */
    //Player
    Player player(texturedStallModel, glm::vec3(100, 0, 50), 0, 0, 0, 1);

    //Camera
    Camera* camera = new Camera(player, glm::vec3(-5.0f, 6.0f, -5.0f));

    /* create renderers */
    MasterRenderer renderer(loader, camera);

    GuiRenderer guiRenderer(loader);

    /* create GUI */
    //GUIText* text = new GUIText("Sample text!", 3, &font, glm::vec2(0, 0), 1.0f, true);

    std::vector<GuiTexture>* guis = new std::vector<GuiTexture>;
    //GuiTexture gui1(loader->loadTexture("res/textures/512px.jpg"), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f, 0.25f));
    //GuiTexture shadowMap(renderer.getShadowMapTexture(), glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.5f));
    //guis->push_back(gui1);
    //guis->push_back(shadowMap);

    /* create particle master */
    particleMaster = new ParticleMaster(loader, renderer.getProjectionMatrix());
    
    /* create mouse picker */
    MousePicker* mousePicker = new MousePicker(*camera, renderer.getProjectionMatrix(), terrain);

    /* create water renderer setup */
    WaterShader* waterShader = new WaterShader();
    WaterFrameBuffers* fbos = new WaterFrameBuffers();
    WaterRenderer* waterRenderer = new WaterRenderer(loader, waterShader, renderer.getProjectionMatrix(), fbos);
    std::vector<WaterTile*> waters;
    WaterTile* water = new WaterTile(120, 140, 2);
    waters.push_back(water);

    /* create particle systems */
    ParticleTexture* particleTexture = new ParticleTexture(loader->loadTexture("res/textures/fire.png"), 8);
    ParticleSystem* system = new ParticleSystem(particleTexture, 50, 25, 0.3f, 4.0f, 5.0f);
    system->randomizeRotation();
    system->setDirection(glm::vec3(0, 1, 0), 0.2f);
    system->setLifeError(0.5f);
    system->setSpeedError(0.5f);
    system->setScaleError(0.5f);

    /* create entities */
    std::vector<Entity*>* entities = new std::vector<Entity*>;
    
    entities->push_back(&player);

    for(int i = 0; i < 300; i++) {
        int treeX = rand() % TERRAIN_SIZE;
        int treeZ = rand() % TERRAIN_SIZE;
        Entity* tree = new Entity(texturedTreeModel, glm::vec3(treeX, terrain->getHeightOfTerrain(treeX, treeZ), treeZ), 0.0, 0.0, 0.0, 5.0);
        entities->push_back(tree);
    }
    
    /* post-processing effects */
    FBO* fbo = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_RENDER_BUFFER);
    if(POST_PROCESSING_ENABLED) POST_PROCESSING_INIT(loader);

    /* Loop until the user closes the window */
    while(!isCloseRequested) {
        //Events
        player.move(*terrain);
        camera->move();

        mousePicker->update();

        //system->generateParticles(glm::vec3(player.getPosition()));
        particleMaster->update(camera);

        //Reset input values
        display.resetInputValues();

        /* Poll for and process events */
        glfwPollEvents();

        // before any rendering takes place, render shadow map
        //renderer.renderShadowMap(entities, sun);

        //OpenGL calls
        glEnable(GL_CLIP_DISTANCE0);

        //Process terrains
        renderer.processTerrain(terrain);

        //Process entities
        renderer.processEntities(entities);

        //Water
        fbos->bindReflectionFrameBuffer();
        float distance = 2 * (camera->getPosition().y - water->getHeight());
        camera->getPosition().y -= distance;
        camera->invertPitch();
        renderer.render(lights, *camera, glm::vec4(0, 1, 0, -water->getHeight() + 1.0f));
        camera->getPosition().y += distance;
        camera->invertPitch();
        fbos->unbindCurrentFrameBuffer();

        fbos->bindRefractionFrameBuffer();
        renderer.render(lights, *camera, glm::vec4(0, -1, 0, water->getHeight() + 1.0f));

        //Draw here
        glDisable(GL_CLIP_DISTANCE0);
        fbos->unbindCurrentFrameBuffer();

        if(POST_PROCESSING_ENABLED) fbo->bindFrameBuffer();
        renderer.render(lights, *camera, glm::vec4(0, -1, 0, 10000));

        waterRenderer->render(waters, *camera, *sun);

        //Particles
        particleMaster->renderParticles(camera);
        
        if(POST_PROCESSING_ENABLED) fbo->unbindFrameBuffer();
        if(POST_PROCESSING_ENABLED) POST_PROCESSING_DRAW(fbo->getColorTexture());

        //Clean up renderer
        renderer.cleanUp();

        //Render GUI
        guiRenderer.render(guis);

        //Render texts
        textMaster.render();

        /* Swap front and back buffers */
        display.updateDisplay();

        //Check if window needs to close
        display.checkCloseRequests();
    }

    //Clean up resources
    loader->cleanUp();
    textMaster.cleanUp();
    fbo->cleanUp();
    if(POST_PROCESSING_ENABLED) POST_PROCESSING_CLEAN_UP();

    delete fbo;

    delete entities;

    delete particleMaster;
    delete waterRenderer;
    delete waterShader;
    delete fbos;

    delete mousePicker;

    delete sun;
    delete light2;
    delete light3;
    delete light4;

    delete guis;

    delete stallModel;

    delete texturePack;

    delete normalMappedLoader;
    delete loader;
    delete camera;
    delete terrain;

    display.closeDisplay();

    return 0;
}