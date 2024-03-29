#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <iostream>
#include <ranges>

#include "toolbox/DisplayManager.h"
#include "renderEngine/MasterRenderer.h"
#include "models/TexturedModel.h"
#include "models/OBJLoader.h"
#include "models/NormalMappingOBJLoader.h"
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

int main(void) {
    /* initialize the glfw library */
    if(!glfwInit())
        return -1;

    DisplayManager::create_display();
    Input::initialize_input();

    /* check OpenGL version */
    std::cout << "OpenGL version supported by this platform: " << glGetString(GL_VERSION) << std::endl;

    /* initialize model loaders */
    auto loader = std::make_shared<Loader>();

    /* create terrain */
    auto background_texture = std::make_unique<TerrainTexture>(loader->load_texture("grass"));
    auto r_texture = std::make_unique<TerrainTexture>(loader->load_texture("mud"));
    auto g_texture = std::make_unique<TerrainTexture>(loader->load_texture("grassFlowers"));
    auto b_texture = std::make_unique<TerrainTexture>(loader->load_texture("path"));
    auto blend_map = std::make_shared<TerrainTexture>(loader->load_texture("black"));

    auto terrain_texture_pack = std::make_shared<TerrainTexturePack>(std::move(background_texture), std::move(r_texture), std::move(g_texture), std::move(b_texture));

    auto terrain = std::make_shared<Terrain>(0, 0, loader, std::move(terrain_texture_pack), std::move(blend_map));

    /* load 3d models */
    auto stall_model = OBJLoader::load_obj("stall", loader);
    auto stall_texture = std::make_shared<ModelTexture>(loader->load_texture("stallTexture"));
    auto textured_stall_model = std::make_shared<TexturedModel>(stall_model, stall_texture);

    auto dragon_model = OBJLoader::load_obj("dragon", loader);
    auto dragon_texture = std::make_shared<ModelTexture>(loader->load_texture("white"));
    auto textured_dragon_model = std::make_shared<TexturedModel>(dragon_model, dragon_texture);

    auto cherry_tree_model = OBJLoader::load_obj("cherry", loader);
    auto cherry_tree_texture = std::make_shared<ModelTexture>(loader->load_texture("cherry"));
    auto textured_cherry_tree_model = std::make_shared<TexturedModel>(cherry_tree_model, cherry_tree_texture);

    auto lantern_model = OBJLoader::load_obj("lantern", loader);
    auto lantern_texture = std::make_shared<ModelTexture>(loader->load_texture("lantern"));
    lantern_texture->set_specular_map(loader->load_texture("lanternS"));
    auto textured_lantern_model = std::make_shared<TexturedModel>(lantern_model, lantern_texture);

    auto barrel_model = NormalMappingOBJLoader::load_normal_mapped_obj("barrel", loader);
    auto barrel_texture = std::make_shared<ModelTexture>(loader->load_texture("barrel"));
    barrel_texture->set_normal_map(loader->load_texture("barrelNormal"));
    barrel_texture->set_specular_map(loader->load_texture("barrelS"));
    auto textured_barrel_model = std::make_shared<TexturedModel>(barrel_model, barrel_texture);

    auto rock_model = NormalMappingOBJLoader::load_normal_mapped_obj("newRock", loader);
    auto rock_texture = std::make_shared<ModelTexture>(loader->load_texture("rockDiffuse"));
    rock_texture->set_normal_map(loader->load_texture("rockNormal"));
    auto textured_rock_model = std::make_shared<TexturedModel>(rock_model, rock_texture);

    /* create light objects */
    auto sun    = std::make_shared<Light>(glm::vec3(0, 20000, 0), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1, 0.001, 0.00001));
    auto light1 = std::make_shared<Light>(glm::vec3(270, 12, 228), glm::vec3(0.65,0.65,0.65), glm::vec3(1, 0.01f, 0.002f));

    std::vector<std::shared_ptr<Light>> lights;
    lights.push_back(light1);

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
    std::vector<std::shared_ptr<Entity>> normal_mapped_entities;
	entities.push_back(player);
    
    auto lantern = std::make_shared<Entity>(textured_lantern_model, glm::vec3(270, 0, 234), 0, 0, 0, 1);
    entities.push_back(lantern);

    auto dragon = std::make_shared<Entity>(textured_dragon_model, glm::vec3(270, 2, 215), 0, 0, 0, 1);
    entities.push_back(dragon);

    auto barrel = std::make_shared<Entity>(textured_barrel_model, glm::vec3(282, 2, 229), 0, 90.f, 90.f, 1);
    normal_mapped_entities.push_back(barrel);

    auto rock = std::make_shared<Entity>(textured_rock_model, glm::vec3(265, 1, 230), 0, 0, 0, 3);
    normal_mapped_entities.push_back(rock);

    /* create tree objects */
    std::uniform_real_distribution<float> map_distribution(0.f, TERRAIN_SIZE);
    std::uniform_real_distribution<float> tree_rotation(0.f, 180.f);

    /* predictable sequence of random values is what we want, because we are able to add other objects without collisions */
	auto random_number_engine = std::mt19937{500};
    auto map_distribution_generator = [&]() { return map_distribution(random_number_engine); };
    auto tree_rotation_generator = [&]() { return tree_rotation(random_number_engine); };
    for(const int i : std::views::iota(0, 300)) {
    	float tree_x, tree_z;

        do {
        	tree_x = map_distribution_generator(),
        	tree_z = map_distribution_generator();
        } while(terrain->get_height_of_terrain(tree_x, tree_z) <= 0);

        float tree_rot_y = tree_rotation_generator();
        entities.push_back(std::make_shared<Entity>(textured_cherry_tree_model, glm::vec3(tree_x, terrain->get_height_of_terrain(tree_x, tree_z), tree_z), 0.0, tree_rot_y, 0.0, 5.0));
    }
    
    /* post-processing effects */
    auto post_processing = std::make_unique<PostProcessing>(loader);

    auto multi_sample_fbo = std::make_unique<FBO>(DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT);
    auto output_fbo1 = std::make_unique<FBO>(DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);
    auto output_fbo2 = std::make_unique<FBO>(DisplayManager::WINDOW_WIDTH, DisplayManager::WINDOW_HEIGHT, FBO_DEPTH_TEXTURE);

    /* Loop until the user closes the window */
    while(!DisplayManager::is_close_requested) {
    	/* Events */
        player->move(terrain, camera->get_yaw(), camera->get_pitch());
        camera->move();

        /* Reset input values */
        DisplayManager::reset_input_values();

        /* Poll for and process events */
        glfwPollEvents();

        /* OpenGL calls */
        glEnable(GL_CLIP_DISTANCE0);

        /* Process terrain objects */
        master_renderer->process_terrain(terrain);

        /* Process entities */
        master_renderer->process_entities(entities);
        master_renderer->process_normal_map_entities(normal_mapped_entities);

        /* Water rendering */
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

        /* Draw here */
        glDisable(GL_CLIP_DISTANCE0);
        water_frame_buffers->unbind_current_frame_buffer();

        multi_sample_fbo->bind_frame_buffer();
        master_renderer->render(lights, camera, glm::vec4(0, -1, 0, 10000));

        water_renderer->render(water_tiles, camera, sun);
        
        multi_sample_fbo->unbind_frame_buffer();
        multi_sample_fbo->resolve_to_fbo(GL_COLOR_ATTACHMENT0, output_fbo1);
        multi_sample_fbo->resolve_to_fbo(GL_COLOR_ATTACHMENT1, output_fbo2);
        post_processing->draw(output_fbo1->get_color_texture(), output_fbo2->get_color_texture());

        /* Clean up renderer */
        master_renderer->clean_up_objects_maps();

        /*  Swap front and back buffers */
        DisplayManager::update_display();

        /* Check if window needs to close */
        DisplayManager::check_close_requests();
    }

    DisplayManager::close_display();

    return 0;
}