#version 460 core

//Shader inputs
in vec2 FragTexturePos;

//uniform inputs
uniform sampler2D ImageTexture;

//output
out vec4 FinalColour;

void main()
{		
		FinalColour = texture(ImageTexture, FragTexturePos);
}