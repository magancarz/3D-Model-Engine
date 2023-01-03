#pragma once

#include "terrain/Terrain.h"
#include "Entity.h"

class Player : public Entity {
public:
	Player(
		std::shared_ptr<TexturedModel> model,
		const glm::vec3& position,
		float rot_x, float rot_y, float rot_z, float scale);

	void move(const std::shared_ptr<Terrain>& terrain, float camera_yaw, float camera_pitch);

	void set_front(const glm::vec3& camera_front);
	void set_right(const glm::vec3& camera_right);

private:
	void check_inputs();
	void jump();

	inline static const float RUN_SPEED  = 20.0f,
						      TURN_SPEED = 160.0f,
						      JUMP_POWER = 30.0f;

	float m_current_speed_x = 0,
		  m_current_speed_z = 0,
		  m_current_turn_speed = 0,
		  m_upwards_speed = 0;

	glm::vec3 m_front = glm::vec3(0.f, 0.f, 1.f),
			  m_right = glm::vec3(-1.f, 0.f, 0.f);
};