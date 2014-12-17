#version 330

uniform sampler2D gSampler;
uniform vec3 lightDirection;
uniform vec3 ambientLightColor;
uniform vec3 diffuseLightColor;

in vec2 texCoord;
in vec3 normal;
out vec4 outputColor;

void main()
{
	outputColor = texture2D(gSampler, texCoord);
}