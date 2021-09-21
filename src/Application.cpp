#include "Headers.h"

#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"
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
    StaticShader shader;
    Renderer renderer(shader);
    Camera camera;
    camera.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    //Load models
    RawModel stallModel = loadObjModel("res/models/stall.obj", loader);
    ModelTexture stallTexture(loader.loadTexture("res/models/stallTexture.png"));
    TexturedModel texturedStallModel(stallModel, stallTexture);
    Entity stall(texturedStallModel, glm::vec3(0, 0, 0), 0, 0, 0, 1);

    /* Loop until the user closes the window */
    while(!isCloseRequested) {
        //Events
        camera.move();

        /* Poll for and process events */
        glfwPollEvents();

        //Prepare window for drawing
        renderer.prepare();

        //Draw here
        shader.start();
        shader.loadViewMatrix(camera);
        renderer.render(stall, shader);
        shader.stop();

        /* Swap front and back buffers */
        display.updateDisplay();

        //Check if window needs to close
        display.checkCloseRequests();
    }

    //Clean up resources
    shader.stop();
    shader.cleanUp();
    loader.cleanUp();

    display.closeDisplay();
    return 0;
}