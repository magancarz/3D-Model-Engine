#include "Headers.h"

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/MasterRenderer.h"
#include "renderEngine/OBJLoader.h"
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

///GLOBAL VARIABLES///
//Main loop control
bool isCloseRequested = false;

//Input manager
Input inputManager;

//Display
DisplayManager display;

int main(void) {
    /* Initialize the library */
    if(!glfwInit())
        return -1;

    //Create window
    display.createDisplay();

    //Logic
    Loader* loader = new Loader;

    //Terrain
    TerrainTexture backgroundTexture(loader->loadTexture("res/textures/grassy2.png"));
    TerrainTexture rTexture(loader->loadTexture("res/textures/mud.png"));
    TerrainTexture gTexture(loader->loadTexture("res/textures/grassFlowers.png"));
    TerrainTexture bTexture(loader->loadTexture("res/textures/path.png"));

    TerrainTexturePack* texturePack = new TerrainTexturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap(loader->loadTexture("res/textures/blendMap.png"));

    Terrain* terrain1 = new Terrain(0, 0, loader, texturePack, &blendMap);

    //Load models
    RawModel* stallModel = loadOBJ("res/models/stall.obj", *loader);
    ModelTexture stallTexture(loader->loadTexture("res/models/stallTexture.png"));
    TexturedModel texturedStallModel(*stallModel, stallTexture);
    ModelTexture& texture = texturedStallModel.getTexture();
    //texture.setShineDamper(10);
    //texture.setReflectivity(5.0f);
    Entity stall(texturedStallModel, glm::vec3(10, 0, 10), 0, 0, 0, 1);
    
    //Light
    Light* light1 = new Light(glm::vec3(0, 1000, -7000), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1, 0, 0));
    Light* light2 = new Light(glm::vec3(10, 0, 10), glm::vec3(1,0,0), glm::vec3(1, 0.01f, 0.002f));
    Light* light3 = new Light(glm::vec3(20, 0, 10), glm::vec3(0,1,0), glm::vec3(1, 0.01f, 0.002f));
    Light* light4 = new Light(glm::vec3(30, 0, 10), glm::vec3(0,0,1), glm::vec3(1, 0.01f, 0.002f));
    std::vector<Light*> lights;
    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);
    lights.push_back(light4);

    //GUI
    std::vector<GuiTexture>* guis = new std::vector<GuiTexture>;
    GuiTexture gui(loader->loadTexture("res/textures/512px.jpg"), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f, 0.25f));
    guis->push_back(gui);

    //Create renderer
    MasterRenderer renderer(loader);

    GuiRenderer guiRenderer(loader);


    //Create player
    Player player(texturedStallModel, glm::vec3(100, 0, -50), 0, 0, 0, 1);

    //Camera
    Camera* camera = new Camera(player, glm::vec3(-5.0f, 6.0f, -5.0f));
    
    //Mouse picking
    MousePicker* mousePicker = new MousePicker(*camera, renderer.getProjectionMatrix(), terrain1);

    //**********Water renderer setup**********//
    WaterShader* waterShader = new WaterShader();
    WaterRenderer* waterRenderer = new WaterRenderer(loader, waterShader, renderer.getProjectionMatrix());
    std::vector<WaterTile*> waters;
    WaterTile* water = new WaterTile(60, 60, 0);
    waters.push_back(water);

    WaterFrameBuffers* fbos = new WaterFrameBuffers();
    GuiTexture reflection(fbos->getReflectionTexture(), glm::vec2(-0.5f, 0.5f), glm::vec2(0.3f, 0.3f));
    GuiTexture refraction(fbos->getRefractionTexture(), glm::vec2(0.5f, 0.5f), glm::vec2(0.3f, 0.3f));
    guis->push_back(reflection);
    guis->push_back(refraction);

    /* Loop until the user closes the window */
    while(!isCloseRequested) {
        //Events
        player.move(*terrain1);
        camera->move();

        mousePicker->update();
        std::cout << mousePicker->getCurrentRay().x << std::endl;

        //Reset input values
        display.resetInputValues();

        /* Poll for and process events */
        glfwPollEvents();

        //OpenGL calls
        glEnable(GL_CLIP_DISTANCE0);

        //Process terrains
        renderer.processTerrain(terrain1);

        //Process entities
        renderer.processEntity(stall);
        renderer.processEntity(player);
        
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
        fbos->unbindCurrentFrameBuffer();

        glDisable(GL_CLIP_DISTANCE0);

        //Draw here
        renderer.render(lights, *camera, glm::vec4(0, -1, 0, 10000));

        waterRenderer->render(waters, *camera);

        //Clean up renderer
        renderer.cleanUp();

        //Render GUI
        guiRenderer.render(guis);

        /* Swap front and back buffers */
        display.updateDisplay();

        //Check if window needs to close
        display.checkCloseRequests();
    }

    //Clean up resources
    loader->cleanUp();

    delete waterRenderer;
    delete waterShader;
    delete fbos;

    delete mousePicker;

    delete light1;
    delete light2;
    delete light3;
    delete light4;

    delete guis;

    delete stallModel;

    delete texturePack;

    delete loader;
    delete camera;
    delete terrain1;

    display.closeDisplay();

    return 0;
}