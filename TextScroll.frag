#version 460 core

in vec2 FragTexCoords;

uniform sampler2D TextTexture;
uniform vec3 TextColour;
uniform float TextLength;

out vec4 FinalColour;

void main()
{
	float Alpha = texture(TextTexture, FragTexCoords).r;
	FinalColour = vec4(TextColour, Alpha);
}