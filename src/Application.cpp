#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"
#include "models/RawModel.h"
#include "models/TexturedModel.h"
#include "shaders/StaticShader.h"

int main(void) {
    const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 800;
    GLFWwindow* WINDOW;
    /* Initialize the library */
    if(!glfwInit())
        return -1;

    //Create window
    const std::string WINDOW_TITLE = "3D Model Viewer";
    WINDOW = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    if(!WINDOW) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window!");
    }

    glfwMakeContextCurrent(WINDOW);

    if(glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    //Logic
    Loader loader;
    StaticShader shader;
    Renderer renderer(shader);

    float vertices[] = {
                -0.5f,0.5f,-0.5f,
                -0.5f,-0.5f,-0.5f,
                0.5f,-0.5f,-0.5f,
                0.5f,0.5f,-0.5f,

                -0.5f,0.5f,0.5f,
                -0.5f,-0.5f,0.5f,
                0.5f,-0.5f,0.5f,
                0.5f,0.5f,0.5f,

                0.5f,0.5f,-0.5f,
                0.5f,-0.5f,-0.5f,
                0.5f,-0.5f,0.5f,
                0.5f,0.5f,0.5f,

                -0.5f,0.5f,-0.5f,
                -0.5f,-0.5f,-0.5f,
                -0.5f,-0.5f,0.5f,
                -0.5f,0.5f,0.5f,

                -0.5f,0.5f,0.5f,
                -0.5f,0.5f,-0.5f,
                0.5f,0.5f,-0.5f,
                0.5f,0.5f,0.5f,

                -0.5f,-0.5f,0.5f,
                -0.5f,-0.5f,-0.5f,
                0.5f,-0.5f,-0.5f,
                0.5f,-0.5f,0.5f

    };
    unsigned int verticesCount = sizeof(vertices) / sizeof(*vertices);

    unsigned int indices[] = {
                0,1,3,
                3,1,2,
                4,5,7,
                7,5,6,
                8,9,11,
                11,9,10,
                12,13,15,
                15,13,14,
                16,17,19,
                19,17,18,
                20,21,23,
                23,21,22

    };
    unsigned int indicesCount = sizeof(indices) / sizeof(*indices);

    float textureCoords[] = {

                0,0,
                0,1,
                1,1,
                1,0,
                0,0,
                0,1,
                1,1,
                1,0,
                0,0,
                0,1,
                1,1,
                1,0,
                0,0,
                0,1,
                1,1,
                1,0,
                0,0,
                0,1,
                1,1,
                1,0,
                0,0,
                0,1,
                1,1,
                1,0


    };
    unsigned int textureCoordsCount = sizeof(textureCoords) / sizeof(*textureCoords);

    RawModel model = loader.loadToVAO(vertices, verticesCount, textureCoords, textureCoordsCount, indices, indicesCount);
    ModelTexture texture(loader.loadTexture("res/textures/512px.jpg"));
    TexturedModel texturedModel(model, texture);
    Entity entity(texturedModel, glm::vec3(0, 0, 0), 0, 0, 0, 1);
    Camera camera;

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(WINDOW)) {
        //Events
        //entity.increaseRotation(1.0f, 1.0f, 0.0f);
        camera.move();

        /* Poll for and process events */
        glfwPollEvents();

        //Prepare window for drawing
        renderer.prepare();

        //Draw here
        shader.start();
        shader.loadViewMatrix(camera);
        renderer.render(entity, shader);
        shader.stop();

        /* Swap front and back buffers */
        glfwSwapBuffers(WINDOW);
    }

    //Clean up resources
    shader.stop();
    shader.cleanUp();
    loader.cleanUp();

    glfwTerminate();
    return 0;
}