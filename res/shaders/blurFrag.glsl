#version 460 core

precision mediump float;

uniform sampler2D originalTexture;

#define KERNEL_SIZE 11

in vec2 position;
in vec2 textureSize;
in int direction;

out vec4 out_Color;

void main() {
	float kernel[KERNEL_SIZE];
	kernel[0] = 0.0093;
	kernel[1] =	0.028002;
	kernel[2] =	0.065984;
	kernel[3] = 0.121703;
	kernel[4] = 0.175713;
	kernel[5] = 0.198596;
	kernel[6] = 0.175713;
	kernel[7] = 0.121703;
	kernel[8] = 0.065984;
	kernel[9] = 0.028002;
	kernel[10] = 0.0093;

	vec2 textCoord = gl_FragCoord.xy / textureSize;
	vec2 onePixel = ((direction == 0) ? vec2(1.0, 0.0) : vec2(0.0, 1.0)) / textureSize;
	vec4 meanColor = vec4(0);
	int ms = KERNEL_SIZE / 2;
	for (int i = 0; i < KERNEL_SIZE; i++)
	{
		meanColor += texture2D(originalTexture, textCoord  + onePixel*vec2(i - ms))*kernel[i];
	}

	out_Color = meanColor;
}