#pragma once

#include "TerrainTexture.h"

class TerrainTexturePack {
public:
	TerrainTexturePack(TerrainTexture& backgroundTexture, TerrainTexture& rTexture,
		TerrainTexture& gTexture, TerrainTexture& bTexture)
		: m_backgroundTexture(backgroundTexture), m_rTexture(rTexture), m_gTexture(gTexture), m_bTexture(bTexture) {}

	inline TerrainTexture& getBackgroundTexture() { return m_backgroundTexture; };
	inline TerrainTexture& getrTexture() { return m_rTexture; };
	inline TerrainTexture& getgTexture() { return m_gTexture; };
	inline TerrainTexture& getbTexture() { return m_bTexture; };
private:
	TerrainTexture& m_backgroundTexture;
	TerrainTexture& m_rTexture;
	TerrainTexture& m_gTexture;
	TerrainTexture& m_bTexture;
};