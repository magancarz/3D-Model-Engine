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
#include "debugging/Debugging.h"

#define DEBUG_ENABLED false

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
bool POST_PROCESSING_ENABLED = true;

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

int main(void) {
    /* initialize the glfw library */
    if(!glfwInit())
        return -1;

    /* initialize display */
    display.createDisplay();

    /* enable debugging */
    if(DEBUG_ENABLED) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    }

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
    TerrainTexture blendMap(loader->loadTexture("res/textures/black.png"));

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

    RawModel* cherryTreeModel = loadOBJ("res/models/cherry.obj", loader);
    ModelTexture cherryTreeTexture(loader->loadTexture("res/textures/cherry.png"));
    TexturedModel texturedCherryTreeModel(*cherryTreeModel, cherryTreeTexture);
    //texturedCherryTreeModel.getTexture().setShineDamper(10);
    //texturedCherryTreeModel.getTexture().setReflectivity(0.5f);
    //texturedCherryTreeModel.getTexture().setSpecularMap(loader->loadTexture("res/textures/cherryS.png"));

    RawModel* lanternModel = loadOBJ("res/models/lantern.obj", loader);
    ModelTexture lanternTexture(loader->loadTexture("res/textures/lantern.png"));
    TexturedModel texturedLanternModel(*lanternModel, lanternTexture);
    texturedLanternModel.getTexture().setSpecularMap(loader->loadTexture("res/textures/lanternS.png"));

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
    Light* sun = new Light(glm::vec3(0, 20000, 0), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1, 0.1f, 0.01f));
    Light* light2 = new Light(glm::vec3(278, 12, 224), glm::vec3(0.65,0.65,0.65), glm::vec3(1, 0.01f, 0.002f));
    Light* light3 = new Light(glm::vec3(20, 0, 10), glm::vec3(0,1,0), glm::vec3(1, 0.01f, 0.002f));
    Light* light4 = new Light(glm::vec3(30, 0, 10), glm::vec3(0,0,1), glm::vec3(1, 0.01f, 0.002f));

    std::vector<Light*> lights;
    lights.push_back(sun);
    lights.push_back(light2);
    lights.push_back(light3);
    lights.push_back(light4);

    /* create player and camera */
    //Player
    Player player(texturedStallModel, glm::vec3(280, 4.5f, 208), 0, 0, 0, 0.000001f);

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
    WaterTile* water = new WaterTile(400, 400, 0);
    waters.push_back(water);
    //GuiTexture waterGui(fbos->getReflectionTexture(), glm::vec2(-0.5f, 0.5f), glm::vec2(0.5f, 0.5f));
    //guis->push_back(waterGui);

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

    Entity* lantern = new Entity(texturedLanternModel, glm::vec3(270, 0, 234), 0, 0, 0, 1);
    entities->push_back(lantern);

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
    FBO* multisampleFBO = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT);
    //FBO* multisampleFBO = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_RENDER_BUFFER);
    FBO* outputFBO1 = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    FBO* outputFBO2 = new FBO(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    if(POST_PROCESSING_ENABLED) POST_PROCESSING_INIT(loader);

    /* Loop until the user closes the window */
    while(!isCloseRequested) {
        //Events
        player.move(*terrain, camera->getYaw(), camera->getPitch());
        camera->move();

        mousePicker->update();

        //system->generateParticles(glm::vec3(player.getPosition()));

        //std::cout << player.getPosition().x << "    " << player.getPosition().y << std::endl;
        //lights[1]->setPosition(glm::vec3(player.getPosition().x, 8.f, player.getPosition().z));

        //Reset input values
        display.resetInputValues();

        /* Poll for and process events */
        glfwPollEvents();

        // before any rendering takes place, render shadow map
        renderer.renderShadowMap(entities, sun);

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

        //Particles
        particleMaster->renderParticles(camera);
        
        multisampleFBO->unbindFrameBuffer();
        multisampleFBO->resolveToFBO(GL_COLOR_ATTACHMENT0, outputFBO1);
        multisampleFBO->resolveToFBO(GL_COLOR_ATTACHMENT1, outputFBO2);
        if(POST_PROCESSING_ENABLED) POST_PROCESSING_DRAW(outputFBO1->getColorTexture(), outputFBO2->getColorTexture());
        //if(POST_PROCESSING_ENABLED) POST_PROCESSING_DRAW(multisampleFBO->getColorTexture());

        //Clean up renderer
        renderer.cleanUp();

        //Render GUI
        //guiRenderer.render(guis);

        //Render texts
        textMaster.render();

        /* Swap front and back buffers */
        display.updateDisplay();

        //Check if window needs to close
        display.checkCloseRequests();

        /* some error checking */
	    //glCheckError();
    }

    //Clean up resources
    loader->cleanUp();
    textMaster.cleanUp();
    multisampleFBO->cleanUp();
    if(POST_PROCESSING_ENABLED) POST_PROCESSING_CLEAN_UP();

    delete outputFBO1;
    delete outputFBO2;
    delete multisampleFBO;

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