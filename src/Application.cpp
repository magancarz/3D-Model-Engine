#include "Headers.h"

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/MasterRenderer.h"
#include "renderEngine/OBJLoader.h"
#include "shaders/StaticShader.h"
#include "models/RawModel.h"
#include "models/TexturedModel.h"
#include "entities/Camera.h"

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
    Camera* camera = new Camera(glm::vec3(-5.0f, 6.0f, -5.0f));

    //Terrain
    ModelTexture* terrainTexture = new ModelTexture(loader->loadTexture("res/textures/grass.png"));
    Terrain* terrain = new Terrain(0, 0, loader, terrainTexture);

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

    //Create renderer
    MasterRenderer renderer;

    /* Loop until the user closes the window */
    while(!isCloseRequested) {
        //Events
        display.rotateCamera(camera);
        camera->move();
        //stall.increaseRotation(0, 1, 0);

        /* Poll for and process events */
        glfwPollEvents();
        
        //Process terrains
        renderer.processTerrain(terrain);

        //Process entities
        renderer.processEntity(stall);
        
        //Draw here
        renderer.render(light, *camera);

        /* Swap front and back buffers */
        display.updateDisplay();

        //Check if window needs to close
        display.checkCloseRequests();
    }

    //Clean up resources
    loader->cleanUp();

    delete stallModel;

    delete loader;
    delete camera;
    delete terrain;

    display.closeDisplay();

    return 0;
}