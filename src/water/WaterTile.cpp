#include "WaterTile.h"

WaterTile::WaterTile(const float center_x, const float center_z, const float height) :
m_height(height), m_x(center_x), m_z(center_z) {}

float WaterTile::get_height() const {
	return m_height;
}

float WaterTile::get_x() const {
	return m_x;
}

float WaterTile::get_z() const {
	return m_z;
}