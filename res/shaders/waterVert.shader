#version 460 core

in vec2 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

const float tiling = 6.0;

void main(void) {
	vec4 worldPosition = model * vec4(position.x, 0.0, position.y, 1.0);
	clipSpace = proj * view * worldPosition;
	gl_Position = clipSpace;
	textureCoords = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;
	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;
}