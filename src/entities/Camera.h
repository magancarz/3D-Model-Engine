#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Player.h"

#define CAMERA_HEIGHT 5

enum direction {FORWARD, BACKWARD, LEFT, RIGHT};

class Camera {
public:
	Camera(std::shared_ptr<Player> player, const glm::vec3& position);

	void move();
	void update_camera_vectors();
	void rotate();

	void invert_pitch();

	glm::mat4 get_view() const;

	void set_position(const glm::vec3& position);
	void set_pitch(float pitch);
	void set_yaw(float yaw);
	void set_roll(float roll);

	void increase_pitch(float pitch);
	void increase_yaw(float yaw);
	void increase_roll(float roll);

	glm::vec3 get_position() const;
	float get_pitch() const;
	float get_yaw() const;
	float get_roll() const;
	float get_sensitivity() const;
	glm::vec3 get_camera_front() const;
	glm::vec3 get_camera_right() const;

private:
	void calculate_camera_position(float horizontal_distance, float vertical_distance);
	float calculate_horizontal_distance() const;
	float calculate_vertical_distance() const;
	void calculate_zoom();
	void calculate_pitch();
	void calculate_angle_around_the_player();

	std::shared_ptr<Player> m_player;

	float m_distance_from_the_player = 50.0f,
		  m_angle_around_the_player = 0;

	glm::vec3 m_position;
	glm::vec3 m_world_up = glm::vec3(0.f, 0.f, 1.f),
	          m_front    = glm::vec3(0.0f, 1.0f, 0.0f),
			  m_right    = glm::vec3(0.0f),
			  m_up       = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_movement_speed, m_sensitivity;
	float m_pitch, m_yaw, m_roll;
};