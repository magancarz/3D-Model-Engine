#include "Headers.h"

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/MasterRenderer.h"
#include "renderEngine/OBJLoader.h"
#include "shaders/StaticShader.h"
#include "models/RawModel.h"
#include "models/TexturedModel.h"
#include "entities/Camera.h"

bool isCloseRequested = false;
Input inputManager;

int main(void) {
    /* Initialize the library */
    if(!glfwInit())
        return -1;

    //Create window
    DisplayManager display;
    display.createDisplay();

    //Logic
    Loader loader;
    Camera camera;
    camera.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    //Load models
    RawModel* stallModel = loadOBJ("res/models/stall.obj", loader);
    ModelTexture stallTexture(loader.loadTexture("res/models/stallTexture.png"));
    TexturedModel texturedStallModel(*stallModel, stallTexture);
    ModelTexture& texture = texturedStallModel.getTexture();
    texture.setShineDamper(10);
    texture.setReflectivity(5.0f);
    Entity stall(texturedStallModel, glm::vec3(0, 0, 0), 0, 0, 0, 1);

    //Light
    Light light(glm::vec3(0, 0, -10), glm::vec3(1,1,1));

    /* Loop until the user closes the window */
    while(!isCloseRequested) {
        //Events
        camera.move();
        //stall.increaseRotation(0, 1, 0);

        /* Poll for and process events */
        glfwPollEvents();

        //Process entities
        renderer.processEntity(stall);

        //Draw here
        renderer.render(light, camera);

        /* Swap front and back buffers */
        display.updateDisplay();

        //Check if window needs to close
        display.checkCloseRequests();
    }

    //Clean up resources
    renderer.cleanUp();
    loader.cleanUp();

    display.closeDisplay();
    return 0;
}