#version 460 core

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

out vec4 out_Color;

uniform sampler2D particleTexture;

void main(void) {
	vec4 color1 = texture(particleTexture, textureCoords1);
	vec4 color2 = texture(particleTexture, textureCoords2);

	out_Color = mix(color1, color2, blend);
}