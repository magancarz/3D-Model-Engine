#pragma once

#include "../Headers.h"
#include "../terrains/Terrain.h"
#include "../renderEngine/DisplayManager.h"
#include "Entity.h"

class Player : public Entity {
public:
	Player(TexturedModel& model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale);

	void move(Terrain& terrain, float cameraYaw, float cameraPitch);

	inline void setFront(const glm::vec3& cameraFront) { m_front = cameraFront; }
	inline void setRight(const glm::vec3& cameraRight) { m_right = cameraRight; }
private:
	void checkInputs();
	void jump();

	const float RUN_SPEED = 20.0f,
				TURN_SPEED = 160.0f,
				JUMP_POWER = 30.0f;

	float m_currentSpeed = 0,
		  m_currentTurnSpeed = 0,
		  m_upwardsSpeed = 0;

	glm::vec3 m_front,
			  m_right;
};