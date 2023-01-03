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

namespace locations {
	std::string res_folder_location = "res/";
    std::string textures_folder_location = "res/textures/";
    std::string image_extension = ".png";
    std::string shader_folder_location = "res/shaders/";
    std::string shader_extension = ".glsl";
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
    auto normal_mapped_obj_loader = std::make_shared<NormalMappingOBJLoader>();

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
    auto stall_texture = std::make_unique<ModelTexture>(loader->loadTexture("res/textures/stallTexture.png"));
    auto textured_stall_model = std::make_shared<TexturedModel>(*stall_model, *stall_texture);

    auto cherry_tree_model = std::unique_ptr<RawModel>(loadOBJ("res/models/cherry.obj", loader.get()));
    auto cherry_tree_texture = std::make_unique<ModelTexture>(loader->loadTexture("res/textures/cherry.png"));
    auto textured_cherry_tree_model = std::make_shared<TexturedModel>(*cherry_tree_model, *cherry_tree_texture);

    auto lantern_model = std::unique_ptr<RawModel>(loadOBJ("res/models/lantern.obj", loader.get()));
    auto lantern_texture = std::make_unique<ModelTexture>(loader->loadTexture("res/textures/lantern.png"));
    auto textured_lantern_model = std::make_shared<TexturedModel>(*lantern_model, *lantern_texture);
    textured_lantern_model->getTexture().setSpecularMap(loader->loadTexture("res/textures/lanternS.png"));

    auto barrel_model = std::unique_ptr<RawModel>(normal_mapped_obj_loader->loadNormalMappedOBJ("res/models/barrel.obj", *loader));
    auto barrel_texture = std::make_unique<ModelTexture>(loader->loadTexture("res/textures/barrel.png"));
    auto textured_barrel_model = std::make_shared<TexturedModel>(*barrel_model, *barrel_texture);
    auto& texture2 = textured_barrel_model->getTexture();
    texture2.setNormalMap(loader->loadTexture("res/textures/barrelNormal.png"));
    texture2.setSpecularMap(loader->loadTexture("res/textures/barrelS.png"));

    /* create light objects */
    auto sun    = std::make_shared<Light>(glm::vec3(0, 20000, 0), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1, 0.1f, 0.01f));
    auto light1 = std::make_shared<Light>(glm::vec3(278, 12, 224), glm::vec3(0.65,0.65,0.65), glm::vec3(1, 0.01f, 0.002f));
    auto light2 = std::make_shared<Light>(glm::vec3(0, 0, 0), glm::vec3(0.65,0.65,0.65), glm::vec3(1, 0.01f, 0.002f));
    auto light3 = std::make_shared<Light>(glm::vec3(0, 0, 0), glm::vec3(0.65,0.65,0.65), glm::vec3(1, 0.01f, 0.002f));

    std::vector<std::shared_ptr<Light>> lights;
    lights.push_back(sun);
    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);

    /* create player and camera */
    auto player = std::make_shared<Player>(textured_stall_model, glm::vec3(280, 4.5f, 208), 0, 0, 0, 0.000001f);

    auto camera = std::make_shared<Camera>(player, glm::vec3(-5.0f, 6.0f, -5.0f));

    /* create master renderer */
    auto master_renderer = std::make_unique<MasterRenderer>(loader, camera);

    /* create water renderer */
    auto water_frame_buffers = std::make_shared<WaterFrameBuffers>();
    auto water_renderer = std::make_unique<WaterRenderer>(loader, water_frame_buffers, master_renderer->get_projection_matrix());
    std::vector<std::shared_ptr<WaterTile>> water_tiles;
    auto water_tile = std::make_shared<WaterTile>(400, 400, 0);
    water_tiles.push_back(water_tile);

    /* create entities */
    std::vector<std::shared_ptr<Entity>> entities;
	entities.push_back(player);
    
    auto lantern = std::make_shared<Entity>(textured_lantern_model, glm::vec3(270, 0, 234), 0, 0, 0, 1);
    entities.push_back(lantern);

    auto barrel = std::make_shared<Entity>(textured_barrel_model, glm::vec3(270, 0, 229), 0, 0, 0, 1);

    /* create tree objects */
    std::uniform_int_distribution<int> map_distribution(0, TERRAIN_SIZE);
    std::uniform_int_distribution<int> tree_rotation(0, 180);

    /* predictable sequence of random values is what we want, because we are able to add other objects without collisions */
	auto random_number_engine = std::mt19937{555};
    auto map_distribution_generator = [&]() { return map_distribution(random_number_engine); };
    auto tree_rotation_generator = [&]() { return tree_rotation(random_number_engine); };
    for(int i = 0; i < 300; i++) {
    	int tree_x, tree_z;

        do {
        	tree_x = map_distribution_generator(),
        	tree_z = map_distribution_generator();
        } while(terrain->get_height_of_terrain(tree_x, tree_z) <= 0);

        int tree_rot_y = tree_rotation_generator();
        entities.push_back(std::make_shared<Entity>(textured_cherry_tree_model, glm::vec3(tree_x, terrain->get_height_of_terrain(tree_x, tree_z), tree_z), 0.0, tree_rot_y, 0.0, 5.0));
    }
    
    /* post-processing effects */
    auto multi_sample_fbo = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT);
    auto output_fbo1 = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    auto output_fbo2 = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    if(post_processing_enabled) POST_PROCESSING_INIT(loader.get());

    /* Loop until the user closes the window */
    while(!DisplayManager::isCloseRequested) {
        //Events
        player->move(terrain, camera->get_yaw(), camera->get_pitch());
        camera->move();

        //Reset input values
        DisplayManager::resetInputValues();

        /* Poll for and process events */
        glfwPollEvents();

        // before any rendering takes place, render shadow map
        master_renderer->render_shadow_map(entities, sun);

        //OpenGL calls
        glEnable(GL_CLIP_DISTANCE0);

        //Process terrains
        master_renderer->process_terrain(terrain);

        //Process entities
        master_renderer->process_entities(entities);
        master_renderer->process_normal_map_entity(barrel);

        //Water
        water_frame_buffers->bind_reflection_frame_buffer();
        const auto camera_position = camera->get_position();
        float distance = 2 * (camera_position.y - water_tile->get_height());
        camera->set_position(glm::vec3(camera_position.x, camera_position.y - distance, camera_position.z));
        camera->invert_pitch();
        master_renderer->render(lights, camera, glm::vec4(0, 1, 0, -water_tile->get_height()));
        camera->set_position(glm::vec3(camera_position.x, camera_position.y, camera_position.z));
        camera->invert_pitch();
        water_frame_buffers->unbind_current_frame_buffer();

        water_frame_buffers->bind_refraction_frame_buffer();
        master_renderer->render(lights, camera, glm::vec4(0, -1, 0, water_tile->get_height()));

        //Draw here
        glDisable(GL_CLIP_DISTANCE0);
        water_frame_buffers->unbind_current_frame_buffer();

        multi_sample_fbo->bindFrameBuffer();
        master_renderer->render(lights, camera, glm::vec4(0, -1, 0, 10000));

        water_renderer->render(water_tiles, camera, sun);
        
        multi_sample_fbo->unbindFrameBuffer();
        multi_sample_fbo->resolveToFBO(GL_COLOR_ATTACHMENT0, output_fbo1.get());
        multi_sample_fbo->resolveToFBO(GL_COLOR_ATTACHMENT1, output_fbo2.get());
        if(post_processing_enabled) POST_PROCESSING_DRAW(output_fbo1->getColorTexture(), output_fbo2->getColorTexture());

        //Clean up renderer
        master_renderer->clean_up_objects_maps();

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