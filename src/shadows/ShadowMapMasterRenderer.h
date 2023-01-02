#pragma once

#include "../entities/Light.h"
#include "ShadowFramebuffer.h"
#include "ShadowShader.h"
#include "ShadowBox.h"
#include "ShadowMapEntityRenderer.h"

class ShadowMapMasterRenderer {
public:
	/**
	* Creates instances of the important objects needed for rendering the scene
	* to the shadow map. This includes the Shadow Box class which calculates
	* the position and size of the "view cuboid", the simple renderer and
	* shader program that are used to render objects to the shadow map, and the
	* Shadow Frame Buffer class to which the scene is rendered. The size of the
	* shadow map is determined here.
	*/
	ShadowMapMasterRenderer(const std::shared_ptr<Camera>& camera);

	/**
	 * Carries out the shadow render pass. This renders the entities to the
	 * shadow map. First the shadow box is updated to calculate the size and
	 * position of the "view cuboid". The light direction is assumed to be
	 * "-lightPosition" which will be fairly accurate assuming that the light is
	 * very far from the scene. It then prepares to render, renders the entities
	 * to the shadow map, and finishes rendering.
	 */
	void render(
		const std::map<std::shared_ptr<TexturedModel>, std::vector<std::shared_ptr<Entity>>>& entities_map,
		const std::shared_ptr<Light>& sun);

	/**
	 * This biased projection-view matrix is used to convert fragments into
	 * "shadow map space" when rendering the main render pass. It converts a
	 * world space position into a 2D coordinate on the shadow map. This is
	 * needed for the second part of shadow mapping.
	 * 
	 * @return The to-shadow-map-space matrix.
	 */
	glm::mat4 get_to_shadow_map_space_matrix() const;

	/**
	* Returns the ID of the shadow map texture. The ID will always stay the
	* same, even when the contents of the shadow map texture change each frame.
	*/
	unsigned int get_shadow_map() const;

	/**
	Returns the light's "view" matrix.
	*/
	glm::mat4 get_light_space_transform() const;

private:
	/**
	Prepare for the shadow render pass. This first updates the dimensions of
	the orthographic "view cuboid" based on the information that was
	calculated in the Shadow Box class. The light's "view" matrix is
	also calculated based on the light's direction and the center position of
	the "view cuboid" which was also calculated in the Shadow Box
	class. These two matrices are multiplied together to create the
	projection-view matrix. This matrix determines the size, position, and
	orientation of the "view cuboid" in the world. This method also binds the
	shadows FBO so that everything rendered after this gets rendered to the
	FBO. It also enables depth testing, and clears any data that is in the
	FBOs depth attachment from last frame. The simple shader program is also
	started.
	 */
	void prepare(const glm::vec3& light_direction);

	/**
	Finish the shadow render pass. Stops the shader and unbinds the shadow
	FBO, so everything rendered after this point is rendered to the screen,
	rather than to the shadow FBO.
	*/
	void finish() const;

	/**
	Updates the "view" matrix of the light. This creates a view matrix which
	will line up the direction of the "view cuboid" with the direction of the
	light. The light itself has no position, so the "view" matrix is centered
	at the center of the "view cuboid". The created view matrix determines
	where and how the "view cuboid" is positioned in the world. The size of
	the view cuboid, however, is determined by the projection matrix.
	 */
	void update_light_view_matrix(const glm::vec3& direction, const glm::vec3& center);

	/**
	 * Creates the orthographic projection matrix. This projection matrix
	 * basically sets the width, length and height of the "view cuboid", based
	 * on the values that were calculated in the Shadow Box class.
	 */
	void update_ortho_projection_matrix(float width, float height, float length);

	/**
	 * Create the offset for part of the conversion to shadow map space. This
	 * conversion is necessary to convert from one coordinate system to the
	 * coordinate system that we can use to sample to shadow map.
	 */
	static glm::mat4 create_offset();

	const int SHADOW_MAP_SIZE = 4096;

	std::unique_ptr<ShadowFrameBuffer> m_shadow_fbo;
	std::shared_ptr<ShadowShader> m_shader;
	std::unique_ptr<ShadowBox> m_shadow_box;
	std::unique_ptr<ShadowMapEntityRenderer> m_shadow_map_entity_renderer;

	glm::mat4 m_projection_matrix      = glm::mat4(1.0f),
			  m_light_view_matrix      = glm::mat4(1.0f),
			  m_projection_view_matrix = glm::mat4(1.0f),
			  m_offset                 = glm::mat4(1.0f);
};