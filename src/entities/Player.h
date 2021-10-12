#pragma once

#include "Headers.h"
#include "Entity.h"

class Player : public Entity {
public:
	Player(TexturedModel& model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale);

	void move();
private:
	void checkInputs();
	void jump();

	const float RUN_SPEED = 20.0f,
				TURN_SPEED = 160.0f,
				JUMP_POWER = 30.0f;

	float m_currentSpeed = 0,
		  m_currentTurnSpeed = 0,
		  m_upwardsSpeed = 0;
};