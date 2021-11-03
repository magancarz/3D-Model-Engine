#version 460 core

in vec4 clipSpace;

out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main(void) {
	vec2 ndc = clipSpace.xy / clipSpace.w / 2.0 + 0.5;
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y);

	vec4 reflectColor = texture(reflectionTexture, reflectionTexCoords);
	vec4 refractColor = texture(refractionTexture, refractionTexCoords);
	
	out_Color = mix(reflectColor, refractColor, 0.5);
}