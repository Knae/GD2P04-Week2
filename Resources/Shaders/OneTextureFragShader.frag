#version 460 core

//Shader inputs
in vec2 FragTexturePos;
//uniform inputs
uniform sampler2D ImageTexture1;
//output
out vec4 FinalColour;

void main()
{
		FinalColour = texture(ImageTexture1, FragTexturePos);
}