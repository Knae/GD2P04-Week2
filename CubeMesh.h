
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CCubeMsh
Description : Singleton mesh class for cube shapes
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#ifndef ___CUBEMESH_H___
#define ___CUBEMESH_H___

#include "Shape.h"
class CCubeMesh :
    public CShape
{
public:
	static CCubeMesh& GetRef() {
		if (m_pThisShape == nullptr)
		{
			m_pThisShape = new CCubeMesh();
		}
		return *m_pThisShape;
	}

	bool SetupShape();
	bool Draw();

private:
	CCubeMesh();
	~CCubeMesh();
	CCubeMesh(const CCubeMesh&) = delete;

	const GLfloat m_Vertices[120] = {
		//vertices position		//texture pos	//indices	//PositionIndex
		//Front
		-0.5f,   0.5f,	 0.5f,	0.0f,	 1.0f,	//00		//0
		-0.5f,  -0.5f,	 0.5f,	0.0f,	 0.0f,	//01		//1
		 0.5f,  -0.5f,	 0.5f,	1.0f,	 0.0f,	//02		//2
		 0.5f,   0.5f,	 0.5f,	1.0f,	 1.0f,	//03		//3
		 //Back
		 0.5f,   0.5f,	-0.5f,	0.0f,	 1.0f,	//04		//4
		 0.5f,  -0.5f,	-0.5f,	0.0f,	 0.0f,	//05		//5
		-0.5f,  -0.5f,	-0.5f,	1.0f,	 0.0f,	//06		//6
		-0.5f,   0.5f,	-0.5f,	1.0f,	 1.0f,	//07		//7
		//Right
		 0.5f,   0.5f,	 0.5f,	0.0f,	 1.0f,	//08		//3
		 0.5f,  -0.5f,	 0.5f,	0.0f,	 0.0f,	//09		//2
		 0.5f,  -0.5f,	-0.5f,	1.0f,	 0.0f,	//10		//5
		 0.5f,   0.5f,	-0.5f,	1.0f,	 1.0f,	//11		//4
		//Left
		-0.5f,   0.5f,	-0.5f,	0.0f,	 1.0f,	//12		//7
		-0.5f,  -0.5f,	-0.5f,	0.0f,	 0.0f,	//13		//6
		-0.5f,  -0.5f,	 0.5f,	1.0f,	 0.0f,	//14		//1
		-0.5f,   0.5f,	 0.5f,	1.0f,	 1.0f,	//15		//0
		//Top
		-0.5f,   0.5f,	-0.5f,	0.0f,	 1.0f,	//16		//7
		-0.5f,   0.5f,	 0.5f,	0.0f,	 0.0f,	//17		//0
		 0.5f,   0.5f,	 0.5f,	1.0f,	 0.0f,	//18		//3
		 0.5f,   0.5f,	-0.5f,	1.0f,	 1.0f,	//19		//4
		 //Bottom
		-0.5f,  -0.5f,	 0.5f,	0.0f,	 1.0f,	//14		//1
		-0.5f,  -0.5f,	-0.5f,	0.0f,	 0.0f,	//13		//6
		 0.5f,  -0.5f,	-0.5f,	1.0f,	 0.0f,	//05		//5
		 0.5f,  -0.5f,	 0.5f,	1.0f,	 1.0f,	//09		//2
	};

	const GLuint m_Indices[36] = {
		 0, 1, 2,	//Front
		 0, 2, 3,	//Front
		 4, 5, 6,	//Back
		 4, 6, 7,	//Back
		 8, 9,10,	//right
		 8,10,11,	//Right
		12,13,14,	//Left
		12,14,15,	//Left
		16,17,18,	//top
		16,18,19,	//Top
		20,21,22,	//Bottom
		20,22,23,	//Bottom
	};

	GLuint m_iID_VAO = 0;
	GLuint m_iID_EBO = 0;
	GLuint m_iID_VBO = 0;

	static CCubeMesh* m_pThisShape;
};
#endif // !___CUBEMESH_H___