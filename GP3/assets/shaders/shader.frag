#version 330

in vec2 texCoord;
in vec3 normal;    
out vec4 outputColor;

uniform sampler2D gSampler;

void main()
{
	outputColor = texture2D(gSampler, texCoord);
}