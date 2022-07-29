
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CObjectManager
Description : manager class to manage objects on display,
like shapes and buttons
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#ifndef ___OBJECTMGR_H___
#define ___OBJECTMGR_H___
//=============
#include <vector>
#include <iomanip>

#include "CubeMesh.h"
#include "DiamondMesh.h"
#include "Button.h"
#include "Camera.h"
#include "ShaderLoader.h"

class CObjectManager
{
public:
	enum class EBUTTONTYPE
	{
		NONE,
		CURSOR,
		WIRE,
	};

	enum class ETYPE
	{
		NONE,
		CUBE,
		DIAMOND,
		BUTTON,
		MAXTYPE,
	};

	typedef struct TObject
	{
		ETYPE eType = ETYPE::NONE;
		glm::vec3 v3Position = glm::vec3(0.0f);
		glm::mat4 m4Transform = glm::mat4();
		glm::vec3 v3Scale = glm::vec3(1.0f);
		glm::vec3 v3MoveVector = glm::vec3(0.0f);
		GLuint uiProgramID = 0;
		GLfloat fCurrentRotateAngle = 0.0f;
		short siRotateDir = 0;	//1 : ClockWise; -1 : AntiClockWise; 0 : None
		glm::vec3 v3RotateAxis = glm::vec3(0.0f);
		bool bRotateInPlace = true;
		glm::vec3 v3RotatePoint = glm::vec3(0.0f);
		float fRotationSpeed = 0.0f;
		bool bIsControlled = false;
	} TObj;

	static CObjectManager& GetRef()
	{
		if (m_thisManager == nullptr)
		{
			m_thisManager = new CObjectManager();
		}
		return *m_thisManager;
	}

	void Initialize();
	void CreateObject(	ETYPE _inType,
						glm::vec3 _inPos,
						glm::vec3 _inScale,
						bool _inIsControlled,
						short _inRotateDir,
						glm::vec3 _inRotateAxis,
						bool _inRotateInPlace, 
						glm::vec3 _inRotatePoint,
						float _inRotateSpeed);
	void CreateButton(	EBUTTONTYPE _inType,
						std::string _inName,
						glm::vec3 _inPosition,
						glm::vec3 _inScale,
						unsigned int _inNumberOfFrames,
						unsigned int _inInactiveFrame,
						unsigned int _inActiveFrame
	);

	void UpdateObjects(float _inDeltaTime);
	void MoveControlledObjects(glm::vec3 _inMoveVector);
	void CheckIfMouseInButtons(double _inMouseX, double _inMouseY);
	EBUTTONTYPE CheckClickOnButtons(double _inMouseX, double _inMouseY);
	const bool GetIfButtonActive(std::string _inName);
	void ToggleButtonActiveState(std::string _inName);
	void DrawObjects();

private:
	CObjectManager();
	~CObjectManager();
	CObjectManager(const CObjectManager&) = delete;
	//Private functions
	void SetupTextures();
	void SetupPrograms();
	GLuint LoadImage(const char* _inLocation);
	//Singleton stuff
	static CObjectManager* m_thisManager;
	CCubeMesh& m_pCubeMeshRef = CCubeMesh::GetRef();
	CDiamondMesh& m_pDiamondRef = CDiamondMesh::GetRef();
	CSquare& m_pSquareRef = CSquare::GetRef();
	CCamera& m_pCameraRef = CCamera::GetRef();
	//Variables
	
	//Object collections
	std::vector<TObject*> m_vecObjects;
	std::vector<CButton*> m_vecButtons;
	//Texture FileLocations
	const char* m_kcFileLocation_LaughingMan = "Resources/Textures/laughingMan.png";
	const char* m_kcFileLocation_Awesome = "Resources/Textures/AwesomeFace.png";
	const char* m_kcFileLocation_CursorToggle = "Resources/Textures/CursorToggle.png";
	const char* m_kcFileLocation_WireframeToggle = "Resources/Textures/Wireframe.png";
	//Shader Locations
	const char* m_kcFileLocation_ButtonVertex = "Resources/Shaders/ButtonVertexShader.vert";
	const char* m_kcFileLocation_ColouredBaseVertex = "Resources/Shaders/ColouredBaseVertexShader.vert";
	const char* m_kcFileLocation_TexturedVertex = "Resources/Shaders/TexturedVertexShader.vert";
	const char* m_kcFileLocation_BaseFrag = "Resources/Shaders/BaseFragShader.frag";
	const char* m_kcFileLocation_OneTextureFrag = "Resources/Shaders/OneTextureFragShader.frag";
	//OpenGL IDs
	GLuint m_TextureIDLaughingMan = 0;
	GLuint m_TextureIDAwesome = 0;
	GLuint m_TextureIDCursorToggle = 0;
	GLuint m_TextureIDWireframeToggle = 0;

	//OpenGL Program IDs
	GLuint uiProgramID_Cube_Rotating_Origin = 0;
	GLuint uiProgramID_Diamond_Controlled = 0;
	GLuint uiProgramID_Button = 0;
	//constants
	const float m_fMoveSpeed = 2.0f;

};

#endif // ___OBJECTMGR_H___