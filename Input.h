
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : Input
Description : contains functions related to keyboard inputs
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#ifndef ___INPUT_H__
#define ___INPUT_H__

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>

/// <summary>
/// Gets current state of key presses and returns normalized direction vector
/// based on movement key that have been press(i.e W,A,S,D keys)
/// </summary>
/// <param name="_inWindow"></param>
/// <returns></returns>
inline glm::vec3 ProcessMovement(int _inKeyCode, int _inActionCode)
{
	glm::vec3 directionVector = glm::vec3();
	if (_inActionCode == GLFW_PRESS)
	{
		if (_inKeyCode == GLFW_KEY_W)
		{
			directionVector += glm::vec3(0.0f, 0.0f, 1.0f);
		}
		else if (_inKeyCode == GLFW_KEY_S)
		{
			directionVector += glm::vec3(0.0f, 0.0f, -1.0f);
		}
		else if (_inKeyCode == GLFW_KEY_A)
		{
			directionVector += glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		else if (_inKeyCode == GLFW_KEY_D)
		{
			directionVector += glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else if (_inKeyCode == GLFW_KEY_Q)
		{
			directionVector += glm::vec3(0.0f, -1.0f, 0.0f);
		}
		else if (_inKeyCode == GLFW_KEY_E)
		{
			directionVector += glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}
	else if(_inActionCode == GLFW_RELEASE)
	{
		if (_inKeyCode == GLFW_KEY_W)
		{
			directionVector += glm::vec3(0.0f, 0.0f, -1.0f);
		}
		else if (_inKeyCode == GLFW_KEY_S)
		{
			directionVector += glm::vec3(0.0f, 0.0f, 1.0f);
		}
		else if (_inKeyCode == GLFW_KEY_A)
		{
			directionVector += glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else if (_inKeyCode == GLFW_KEY_D)
		{
			directionVector += glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		else if (_inKeyCode == GLFW_KEY_Q)
		{
			directionVector += glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else if (_inKeyCode == GLFW_KEY_E)
		{
			directionVector += glm::vec3(0.0f, -1.0f, 0.0f);
		}
	}
	return directionVector;
}

/// <summary>
/// Returns the char code of the key depending on if it
/// needs to be uppercase.
/// </summary>
/// <param name="_inKeyCode"></param>
/// <param name="_inUpperCase"></param>
/// <returns></returns>
inline char ParseButtonPressToChar(int _inKeyCode, bool _inUpperCase)
{
	if (_inUpperCase)
	{
		return char(_inKeyCode);
	}
	else
	{
		return char(_inKeyCode + 32);
	}
}

#endif // !___INPUT_H__