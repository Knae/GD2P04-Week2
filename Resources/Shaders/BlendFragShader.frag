#version 460 core

//Shader inputs
in vec2 FragTexturePos;
//uniform inputs
uniform sampler2D ImageTexture1;
uniform sampler2D ImageTexture2;
uniform float ColourModifier;

//output
out vec4 FinalColour;

void main()
{
		FinalColour =	mix(texture(ImageTexture1, FragTexturePos),
							texture(ImageTexture2, FragTexturePos),
							ColourModifier);
}