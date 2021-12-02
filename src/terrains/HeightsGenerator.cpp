#include "HeightsGenerator.h"

HeightsGenerator::HeightsGenerator() {
	m_seed = 44322133;
	std::cout << getNoise(5, 14) << std::endl;
	std::cout << getNoise(6, 14) << std::endl;
	std::cout << getNoise(7, 14) << std::endl;
	std::cout << getNoise(8, 14) << std::endl;
	std::cout << getNoise(9, 14) << std::endl;
}

float HeightsGenerator::interpolate(float a, float b, float blend) {
	double theta = blend * glm::pi<float>();
	float f = (1.0f - glm::cos(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float HeightsGenerator::getInterpolatedNoise(float x, float z) {
	int intX = (int) x;
	int intZ = (int) z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}

float HeightsGenerator::generateHeight(int x, int z) {
	float total = 0;
	float d = (float) glm::pow(2, OCTAVES - 1);
	for(int i = 0; i < OCTAVES; i++) {
		float freq = (float) glm::pow(2, i) * d;
		float amp = (float) glm::pow(ROUGHNESS, i) * AMPLITUDE;
		total += getInterpolatedNoise(x * freq, z * freq) * amp;
	}
	return total;
}

float HeightsGenerator::getNoise(int x, int z) {
	srand(x * 122 + z * 1223 + m_seed * 1233);
	return random() * 2.0f - 1.0f;
}

float HeightsGenerator::getSmoothNoise(int x, int z) {
	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 16.0f;
	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1) + getNoise(x, z + 1)) / 8.0f;
	float center = getNoise(x, z) / 4.0f;
	return corners + sides + center;
}