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
    Light light(glm::vec3(0, 0, -10), glm::vec3(1,1,1));

    //GUI
    std::vector<GuiTexture>* guis = new std::vector<GuiTexture>;
    GuiTexture gui(loader->loadTexture("res/textures/512px.jpg"), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f, 0.25f));
    guis->push_back(gui);

    //Create renderer
    MasterRenderer renderer;

    GuiRenderer guiRenderer(loader);

    //Create player
    Player player(texturedStallModel, glm::vec3(100, 0, -50), 0, 0, 0, 1);

    //Camera
    Camera* camera = new Camera(player, glm::vec3(-5.0f, 6.0f, -5.0f));

    /* Loop until the user closes the window */
    while(!isCloseRequested) {
        //Events
        //camera->rotate();
        camera->move();
        player.move(*terrain1);
        //stall.increaseRotation(0, 1, 0);

        //Reset input values
        display.resetInputValues();

        /* Poll for and process events */
        glfwPollEvents();

        //Process terrains
        renderer.processTerrain(terrain1);

        //Process entities
        renderer.processEntity(stall);
        renderer.processEntity(player);
        
        //Draw here
        renderer.render(light, *camera);

        //Render GUI
        guiRenderer.render(guis);

        /* Swap front and back buffers */
        display.updateDisplay();

        //Check if window needs to close
        display.checkCloseRequests();
    }

    //Clean up resources
    loader->cleanUp();

    delete guis;

    delete stallModel;

    delete texturePack;

    delete loader;
    delete camera;
    delete terrain1;

    display.closeDisplay();

    return 0;
}