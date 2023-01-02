#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <iostream>

#include "toolbox/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/MasterRenderer.h"
#include "renderEngine/OBJLoader.h"
#include "normalMappingRenderer/NormalMappingOBJLoader.h"
#include "models/RawModel.h"
#include "models/TexturedModel.h"
#include "entities/Camera.h"
#include "entities/Player.h"
#include "water/WaterRenderer.h"
#include "water/WaterFrameBuffers.h"
#include "renderEngine/postProcessing/FBO.h"
#include "renderEngine/postProcessing/PostProcessing.h"
#include "terrain/Terrain.h"
#include "textures/TerrainTexture.h"
#include "textures/TerrainTexturePack.h"
#include "toolbox/Input.h"
#include "water/WaterShader.h"

namespace LOCATIONS {
	std::string RES_FOLDER_LOCATION = "res/";
    std::string TEXTURES_FOLDER_LOCATION = "res/textures/";
    std::string IMAGE_EXTENSION = ".png";
}

int main(void) {
	bool post_processing_enabled = true;

    /* initialize the glfw library */
    if(!glfwInit())
        return -1;

    DisplayManager::create_display();
    Input::initialize_input();

    /* check OpenGL version */
    std::cout << "OpenGL version supported by this platform: " << glGetString(GL_VERSION) << std::endl;

    /* initialize model loaders */
    auto loader = std::make_shared<Loader>();
    auto normal_mapped_obj_loader = std::make_unique<NormalMappingOBJLoader>();

    /* create terrain */
    auto background_texture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/grass.png"));
    auto r_texture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/mud.png"));
    auto g_texture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/grassFlowers.png"));
    auto b_texture = std::make_unique<TerrainTexture>(loader->loadTexture("res/textures/path.png"));
    auto blend_map = std::make_shared<TerrainTexture>(loader->loadTexture("res/textures/black.png"));

    auto terrain_texture_pack = std::make_shared<TerrainTexturePack>(std::move(background_texture), std::move(r_texture), std::move(g_texture), std::move(b_texture));

    auto terrain = std::make_shared<Terrain>(0, 0, loader, std::move(terrain_texture_pack), std::move(blend_map));

    /* load 3d models */
    auto stall_model = std::unique_ptr<RawModel>(loadOBJ("res/models/stall.obj", loader.get()));
    ModelTexture stall_texture(loader->loadTexture("res/textures/stallTexture.png"));
    TexturedModel textured_stall_model(*stall_model, stall_texture);

    auto cherry_tree_model = loadOBJ("res/models/cherry.obj", loader.get());
    ModelTexture cherry_tree_texture(loader->loadTexture("res/textures/cherry.png"));
    TexturedModel textured_cherry_tree_model(*cherry_tree_model, cherry_tree_texture);

    auto lantern_model = loadOBJ("res/models/lantern.obj", loader.get());
    ModelTexture lantern_texture(loader->loadTexture("res/textures/lantern.png"));
    TexturedModel textured_lantern_model(*lantern_model, lantern_texture);
    textured_lantern_model.getTexture().setSpecularMap(loader->loadTexture("res/textures/lanternS.png"));
    
    /* create light objects */
    auto sun = std::make_unique<Light>(glm::vec3(0, 20000, 0), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1, 0.1f, 0.01f));
    auto light1 = std::make_unique<Light>(glm::vec3(278, 12, 224), glm::vec3(0.65,0.65,0.65), glm::vec3(1, 0.01f, 0.002f));
    auto light2 = std::make_unique<Light>(glm::vec3(20, 0, 10), glm::vec3(0,1,0), glm::vec3(1, 0.01f, 0.002f));
    auto light3 = std::make_unique<Light>(glm::vec3(30, 0, 10), glm::vec3(0,0,1), glm::vec3(1, 0.01f, 0.002f));

    std::vector<Light*> lights;
    lights.push_back(sun.get());
    lights.push_back(light1.get());
    lights.push_back(light2.get());
    lights.push_back(light3.get());

    /* create player and camera */
    auto player = std::make_unique<Player>(textured_stall_model, glm::vec3(280, 4.5f, 208), 0, 0, 0, 0.000001f);

    auto camera = std::make_unique<Camera>(*player, glm::vec3(-5.0f, 6.0f, -5.0f));

    /* create master renderer */
    MasterRenderer master_renderer(loader.get(), camera.get());

    /* create water renderer */
    auto water_shader = std::make_unique<WaterShader>();
    auto water_frame_buffers = std::make_unique<WaterFrameBuffers>();
    auto water_renderer = std::make_unique<WaterRenderer>(loader.get(), water_shader.get(), master_renderer.getProjectionMatrix(), water_frame_buffers.get());
    std::vector<WaterTile*> water_tiles;
    auto water_tile = std::make_unique<WaterTile>(400, 400, 0);
    water_tiles.push_back(water_tile.get());

    /* create entities */
    auto entities = std::make_unique<std::vector<Entity*>>();
	entities->push_back(player.get());
    
    auto lantern = std::make_unique<Entity>(textured_lantern_model, glm::vec3(270, 0, 234), 0, 0, 0, 1);
    entities->push_back(lantern.get());

    /* create tree objects */
    std::uniform_int_distribution<unsigned int> map_distribution(0, TERRAIN_SIZE);
    std::uniform_int_distribution<int> tree_rotation(0, 180);

    /* predictable sequence of random values is what we want, because we are able to add other objects without collisions */
	auto random_number_engine = std::mt19937{555};
    auto map_distribution_generator = [&]() { return map_distribution(random_number_engine); };
    auto tree_rotation_generator = [&]() { return tree_rotation(random_number_engine); };
    for(int i = 0; i < 300; i++) {
		unsigned int tree_x, tree_z;

        do {
        	tree_x = map_distribution_generator(),
        	tree_z = map_distribution_generator();
        } while(terrain->get_height_of_terrain(tree_x, tree_z) <= 0);

        int tree_rot_y = tree_rotation_generator();
        auto new_tree = new Entity(textured_cherry_tree_model, glm::vec3(tree_x, terrain->get_height_of_terrain(tree_x, tree_z), tree_z), 0.0, tree_rot_y, 0.0, 5.0);
        entities->push_back(new_tree);
    }
    
    /* post-processing effects */
    auto multi_sample_fbo = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT);
    auto output_fbo1 = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    auto output_fbo2 = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    if(post_processing_enabled) POST_PROCESSING_INIT(loader.get());

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
        master_renderer.renderShadowMap(entities.get(), sun.get());

        //OpenGL calls
        glEnable(GL_CLIP_DISTANCE0);

        //Process terrains
        master_renderer.processTerrain(terrain.get());

        //Process entities
        master_renderer.processEntities(entities.get());

        //Water
        water_frame_buffers->bindReflectionFrameBuffer();
        float distance = 2 * (camera->getPosition().y - water_tile->getHeight());
        camera->getPosition().y -= distance;
        camera->invertPitch();
        master_renderer.render(lights, *camera, glm::vec4(0, 1, 0, -water_tile->getHeight()));
        camera->getPosition().y += distance;
        camera->invertPitch();
        water_frame_buffers->unbindCurrentFrameBuffer();

        water_frame_buffers->bindRefractionFrameBuffer();
        master_renderer.render(lights, *camera, glm::vec4(0, -1, 0, water_tile->getHeight()));

        //Draw here
        glDisable(GL_CLIP_DISTANCE0);
        water_frame_buffers->unbindCurrentFrameBuffer();

        multi_sample_fbo->bindFrameBuffer();
        master_renderer.render(lights, *camera, glm::vec4(0, -1, 0, 10000));

        water_renderer->render(water_tiles, *camera, *sun);
        
        multi_sample_fbo->unbindFrameBuffer();
        multi_sample_fbo->resolveToFBO(GL_COLOR_ATTACHMENT0, output_fbo1.get());
        multi_sample_fbo->resolveToFBO(GL_COLOR_ATTACHMENT1, output_fbo2.get());
        if(post_processing_enabled) POST_PROCESSING_DRAW(output_fbo1->getColorTexture(), output_fbo2->getColorTexture());

        //Clean up renderer
        master_renderer.cleanUp();

        /* Swap front and back buffers */
        DisplayManager::updateDisplay();

        //Check if window needs to close
        DisplayManager::checkCloseRequests();
    }

    //Clean up resources
    loader->cleanUp();
    multi_sample_fbo->cleanUp();
    if(post_processing_enabled) POST_PROCESSING_CLEAN_UP();

    DisplayManager::closeDisplay();

    return 0;
}
