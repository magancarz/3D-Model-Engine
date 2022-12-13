#version 460 core

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D textureSampler;

uniform sampler2D specularSampler;
uniform int usesSpecularMap;

uniform vec3 lightColor[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

const float levels = 3.0;

void main(void) {
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for(int i = 0; i < 4; i++) {
		float distance = length(toLightVector[i]);
		float attFactor = attenuation[i].x + (attenuation[i].y * distance) + (attenuation[i].z * distance * distance);
		vec3 unitLightVector = normalize(toLightVector[i]);
		float nDot1 = dot(unitNormal, unitLightVector);
		float brightness = max(nDot1, 0.0);
		float level = floor(brightness * levels);
		brightness = level / levels;
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, shineDamper);
		level = floor(dampedFactor * levels);
		dampedFactor = level / levels;
		totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attFactor;
		totalSpecular = totalSpecular + (dampedFactor * reflectivity * lightColor[i]) / attFactor;
	}

	totalDiffuse = max(totalDiffuse, 0.2);

	vec4 textureColor = texture(textureSampler, pass_textureCoords);
	if(textureColor.a < 0.5) {
		discard;
	}

	if(usesSpecularMap > 0.5) {
		vec4 mapInfo = texture(specularSampler, pass_textureCoords);
		totalSpecular *= mapInfo.r;
		if(mapInfo.g > 0.5) {
			totalDiffuse = vec3(1.0);
		}
	}

	out_Color = vec4(totalDiffuse, 1.0) * texture(textureSampler, pass_textureCoords) + vec4(totalSpecular, 1.0);
	out_Color = mix(vec4(skyColor, 1.0), out_Color, visibility);
	//out_Color = vec4(diffuse, 1.0) * vec4(0.3, 0.3, 0.3, 1.0) + vec4(finalSpecular, 1.0);
}