#version 460 core

in vec3 position;
in vec2 textureCoords;
in vec3 normal;

out vec2 pass_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lightPosition;

void main(void) {
	vec4 worldPosition = model * vec4(position, 1.0);
	gl_Position = proj * view * model * vec4(position, 1.0);
	pass_textureCoords = textureCoords;

	surfaceNormal = (model * vec4(normal, 0.0)).xyz;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}