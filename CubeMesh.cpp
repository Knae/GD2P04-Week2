#include "CubeMesh.h"

CCubeMesh* CCubeMesh::m_pThisShape = nullptr;

/// <summary>
/// Setup hexagon mesh
/// </summary>
/// <returns></returns>
bool CCubeMesh::SetupShape()
{
	try
	{
		//Generate VAO
		glGenVertexArrays(1, &m_iID_VAO);
		glBindVertexArray(m_iID_VAO);
		//Generate EBO
		glGenBuffers(1, &m_iID_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iID_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_STATIC_DRAW);
		//Generate VBO
		glGenBuffers(1, &m_iID_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_iID_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);
		//Set Vertex Attribute info
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		std::cout << "Cube mesh built\n";
		return true;
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to setup Cube mesh. VAO ID: " << m_iID_VAO << "\n";
		return false;
	}
}

/// <summary>
/// Draw hexagon shape at given coordinates
/// </summary>
/// <param name="_inTextureID"></param>
/// <param name="_inRotateDir"></param>
/// <returns></returns>
bool CCubeMesh::Draw()
{
	try
	{
		//Bind VAO
		glBindVertexArray(m_iID_VAO);
		//Draw triangles
		glDrawElements(GL_TRIANGLES, sizeof(m_Indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		//Unbind for other objects
		glBindVertexArray(0);
		return true;
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to draw Cube mesh VAO ID: " << m_iID_VAO << "\n";
		return false;
	}
}

CCubeMesh::CCubeMesh()
{

}

CCubeMesh::~CCubeMesh()
{
	delete m_pThisShape;
	m_pThisShape = nullptr;
}