#version 460 core

in vec2 textureCoords;

out vec4 out_Color;

uniform sampler2D colorTexture;
uniform sampler2D highlightTexture;

void main(void) {
	vec4 sceneColor = texture(colorTexture, textureCoords);
	vec4 highlightColor = texture(highlightTexture, textureCoords);

	out_Color = sceneColor + highlightColor * 3.5;
}