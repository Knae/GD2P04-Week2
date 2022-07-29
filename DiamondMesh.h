
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CDiamondMesh
Description : mesh class for diamond shapes
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#include "Shape.h"
class CDiamondMesh :
    public CShape
{
public:
	static CDiamondMesh& GetRef() {
		if (m_pThisShape == nullptr)
		{
			m_pThisShape = new CDiamondMesh();
		}
		return *m_pThisShape;
	}

	bool SetupShape();
	bool Draw();

private:
	CDiamondMesh();
	~CDiamondMesh();
	CDiamondMesh(const CDiamondMesh&) = delete;

	const GLfloat m_Vertices[30] = {
		//vertices position			//texture pos	//indices	//Points
		 0.0f,   1.0f,	 0.0f,		0.0f, 0.0f,		//00		//Top
		-0.5f,   0.0f,	 0.5f,		0.0f, 0.0f,		//01		//Front Left
		 0.5f,   0.0f,	 0.5f,		0.0f, 0.0f,		//02		//Front Right
		 0.5f,	 0.0f,  -0.5f,		0.0f, 0.0f,		//03		//Back Right
		-0.5f,	 0.0f,  -0.5f,		0.0f, 0.0f,		//04		//Back Left
		 0.0f,	-1.0f,   0.0f,		0.0f, 0.0f,		//05		//Bottom
	};

	const GLuint m_Indices[24] = {
	 0, 1, 2,
	 0, 2, 3,
	 0, 3, 4,
	 0, 4, 1,
	 1, 5, 2,
	 2, 5, 3,
	 3, 5, 4,
	 4, 5, 1,
	};

	GLuint m_iID_VAO = 0;
	GLuint m_iID_EBO = 0;
	GLuint m_iID_VBO = 0;

	static CDiamondMesh* m_pThisShape;
};
