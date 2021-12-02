#pragma once

#include "../Headers.h"

class HeightsGenerator {
public:
	HeightsGenerator();

	float generateHeight(int x, int z);
	//int getVertexCount() { return }

private:
	float interpolate(float a, float b, float blend);
	float getInterpolatedNoise(float x, float z);
	float getNoise(int x, int z);
	float getSmoothNoise(int x, int z);

	//TextureData* image;
	int stepSize,
		vertexCount,
		imageStepSize,
		imageVertexCount;

	const float AMPLITUDE = 70.0f,
				ROUGHNESS = 0.3f;
	const int OCTAVES = 3;
	int m_seed,

	int xmin, xmax, zmin, zmax,
		getNoiseCalls,
		getNoise1Calls,
		getSmoothNoiseCalls,
		getInterpolatedNoiseCalls;

	std::vector<float>* noiseTable;

};