#version 460

//Vertex data arrangement
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 textPos;
//Inputs
uniform mat4 inputMatrix;
//Outputs to Fragment Shader
out vec3 FragColour;


void main()
{
	gl_Position = inputMatrix * vec4(Position, 1.0f);
	FragColour = Position;
}