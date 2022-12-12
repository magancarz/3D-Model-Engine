#pragma once

static constexpr float WATER_TILE_SIZE = 400;

class WaterTile {
public:

	WaterTile(float centerX, float centerZ, float height)
		: m_x(centerX), m_z(centerZ), m_height(height) {}

	inline float getHeight() { return m_height; };
	inline float getX() { return m_x; };
	inline float getZ() { return m_z; };
private:
	float m_height;
	float m_x, m_z;
};