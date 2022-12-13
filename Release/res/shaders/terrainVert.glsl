#version 460 core

in vec3 position;
in vec2 textureCoords;
in vec3 normal;

out vec2 pass_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lightPosition[4];

uniform vec4 plane;

const float density = 0.007;
const float gradient = 1.5;

void main(void) {
	vec4 worldPosition = model * vec4(position, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, plane);

	vec4 positionRelativeToCam = view * worldPosition;
	gl_Position = proj * positionRelativeToCam;
	pass_textureCoords = textureCoords;

	surfaceNormal = (model * vec4(normal, 0.0)).xyz;
	for(int i = 0; i < 4; i++) {
		toLightVector[i] = lightPosition[i] - worldPosition.xyz;
	}
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}