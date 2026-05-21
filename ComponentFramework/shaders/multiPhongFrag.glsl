#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_NUMBER_OF_LIGHTS 10

layout(location = 0) out vec4 fragColor;

layout(location = 0) in vec3 vertNormal;
layout(location = 3) in vec3 lightDir[MAX_NUMBER_OF_LIGHTS]; // should be the same in vert shader
layout(location = 1) in vec3 eyeDir; 

layout(location = 2) in vec2 texCoords;

uniform sampler2D someTexture;

uniform vec4 diffuseLight[MAX_NUMBER_OF_LIGHTS];
uniform vec4 spectularLight[MAX_NUMBER_OF_LIGHTS];
uniform float brightness[MAX_NUMBER_OF_LIGHTS];
uniform int numberOfDiffuseLight;
uniform int numberOfSpectularLight;
uniform int numberOfBrightness;

void main() {
	// the index of light should be bound with the index of it's color
	for(int i = 0; i < numberOfDiffuseLight; i++){
		vec4 ks = spectularLight[i]; // color of light
		vec4 kd = diffuseLight[i]; // color of object
		vec4 ka = brightness[i] * kd; // brightness

		vec4 textureColor = texture(someTexture, texCoords);

		float diff = max(dot(vertNormal, lightDir[i]), 0.0);

		/// Reflection is based incedent direction which means a vector from the light source
		/// not the direction to the light source
		vec3 reflection = normalize(reflect(-lightDir[i], vertNormal));
		float spec = max(dot(eyeDir, reflection), 0.0);
		spec = pow(spec, 3.0);
		fragColor +=  (ka + (diff * kd) + (spec * ks));	
	}
}