#version 460

//Vertex data arrangement
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexturePos;
//Outputs to Fragment Shader
out vec2 FragTexturePos;
//Inputs
uniform mat4 inputMatrix;

void main()
{
	gl_Position = inputMatrix * vec4(Position, 1.0f);
	FragTexturePos = TexturePos;
}