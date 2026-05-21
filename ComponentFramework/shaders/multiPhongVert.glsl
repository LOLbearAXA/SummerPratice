#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_NUMBER_OF_LIGHTS 10 // should be the same in C++

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos[MAX_NUMBER_OF_LIGHTS];
uniform int numberOfLight;

layout(location = 0) out vec3 vertNormal;
// just been lazy on setting the pipe line location
// if location is at 1, then lightDir is going to take all the 
// pipe location from 1 to 1 + MAX_NUMBER_OF_LIGHTS - 1
// so the next avalible location will be 11
layout(location = 3) out vec3 lightDir[MAX_NUMBER_OF_LIGHTS];
layout(location = 1) out vec3 eyeDir; 

layout(location = 2) out vec2 texCoords;
void main() {
    texCoords = uvCoord;
    texCoords.y *= -1;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;
    for(int i = 0; i < numberOfLight; i++){
        lightDir[i] = normalize(vec3(lightPos[i]) - vertPos); 
    }
	/// vVertex is in Model space 
	/// modelMatrix * vVertex is in World space
	/// viewMatrix * modelMatrix * vVertex is in Camera space 
	/// projectionMatrix * viewMatrix * modelMatrix * vVertex is in NDC space 
	/// I'll explain spaces in class. 
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
    
}
