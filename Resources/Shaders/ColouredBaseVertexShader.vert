#version 460

//Vertex data arrangement
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 textPos;
//Inputs
uniform mat4 inputMatrix;
uniform vec3 Colour;
uniform int UseInputColour = 0;
//Outputs to Fragment Shader
out vec3 FragColour;


void main()
{
	gl_Position = inputMatrix * vec4(Position, 1.0f);
	if(UseInputColour > 0)
	{
		FragColour = Colour;
	}
	else
	{
		FragColour = Position;
	}
}