#pragma once

class WaterTile {
public:
	WaterTile(float center_x, float center_z, float height);

	float get_height() const;
	float get_x() const;
	float get_z() const;

	inline static const float WATER_TILE_SIZE = 400;
private:
	float m_height;
	float m_x, m_z;
};