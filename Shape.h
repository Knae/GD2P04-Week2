
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CShape
Description : parent class for shapes. ensures that all shapes
have a setupShape and a Draw function
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
//==========
//Includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
//==========

class CShape
{
public:
	virtual bool SetupShape() = 0;
	virtual bool Draw() = 0;
private:

};

