#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inCoord;

out vec2 texCoord;
out vec3 normal;

void main()
{
	texCoord = inCoord;
	mat4 worldView = viewMatrix * worldMatrix;
	gl_Position = projectionMatrix * worldView * vec4(inPosition, 1.0);
    normal = (worldView * vec4(inNormal, 0.0)).xyz;
}