#version 460

//Vertex data arrangement
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Colour;
//Outputs to Fragment Shader
out vec3 FragColour;
//Inputs
uniform mat4 inputMatrix;

void main()
{
	gl_Position = inputMatrix * vec4(Position, 1.0f);
	FragColour = Colour;
}