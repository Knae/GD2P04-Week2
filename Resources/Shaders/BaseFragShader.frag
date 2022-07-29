#version 460 core

//Shader inputs
in vec3 FragColour;

//output
out vec4 FinalColour;

void main()
{
		FinalColour =	vec4(FragColour,1.0f);
}