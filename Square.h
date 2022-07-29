
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CSquare
Description : singleton Mesh class for square buttons
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#include "Shape.h"
class CSquare :
    public CShape
{
public:
	static CSquare& GetRef() {
		if (m_pThisShape == nullptr)
		{
			m_pThisShape = new CSquare();
		}
		return *m_pThisShape;
	}

	bool SetupShape();
	bool Draw();
private:
	CSquare();
	~CSquare();
	CSquare(const CSquare&) = delete;

	const GLfloat m_Vertices[20] = {
		//vertices position		//texture pos	//indices
		-0.5f,   0.5f,	 0.0f,	0.0f,	 1.0f,	//00	
		-0.5f,  -0.5f,	 0.0f,	0.0f,	 0.0f,	//01	
		 0.5f,  -0.5f,	 0.0f,	1.0f,	 0.0f,	//02	
		 0.5f,   0.5f,	 0.0f,	1.0f,	 1.0f,	//03	

	};

	const GLuint m_Indices[6] = {
		 0, 1, 2,
		 0, 2, 3,
	};


	GLuint m_iID_VAO = 0;
	GLuint m_iID_EBO = 0;
	GLuint m_iID_VBO = 0;

	static CSquare* m_pThisShape;
};

