#pragma once

#include "terrain/Terrain.h"
#include "Entity.h"

class Player : public Entity {
public:
	Player(std::shared_ptr<TexturedModel> model, const glm::vec3& position, float rot_x, float rot_y, float rot_z, float scale);

	void move(Terrain& terrain, float cameraYaw, float cameraPitch);

	inline void setFront(const glm::vec3& cameraFront) { m_front = cameraFront; }
	inline void setRight(const glm::vec3& cameraRight) { m_right = cameraRight; }
private:
	void checkInputs();
	void jump();

	const float RUN_SPEED = 20.0f,
				TURN_SPEED = 160.0f,
				JUMP_POWER = 30.0f;

	float m_currentSpeedX = 0,
		  m_currentSpeedZ = 0,
		  m_currentTurnSpeed = 0,
		  m_upwardsSpeed = 0,
		  m_canJump = true;

	glm::vec3 m_front,
			  m_right;
};