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
	float lambert = max(dot(lightDirection, normal), 0.0);	
	outputColor = texture2D(gSampler, texCoord) * 
					vec4(ambientLightColor + 
						lambert * diffuseLightColor
						, 1.0);
}