#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrix;
//uniform mat4 modelViewMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inCoord;

out vec2 texCoord;
out vec3 normal;

void main()
{
	gl_Position = projectionMatrix*viewMatrix*worldMatrix*vec4(inPosition, 1.0);
	texCoord = inCoord;
    normal = (worldMatrix * vec4(normal, 0.0)).xyz;
}