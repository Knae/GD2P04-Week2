
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CButton
Description : Class for creating a button object in a
SFML system
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#ifndef ___BUTTON_H___
#define ___BUTTON_H___
#include "Square.h"
#include "Utility.h"
#include "Camera.h"

class CButton
{
public:
	CButton(std::string _inName,
			glm::vec3 _inPosition,
			glm::vec3 _inScale,
			unsigned int _inNumberOfFrames,
			unsigned int _inInactiveFrame,
			unsigned int _inActiveFrame,
			GLuint _inTextureID
		);
	~CButton();

	void Update(float _inDeltaTime);
	void Render();
	bool CheckIfMouseWithinButton(double _inPosX, double _inPosY);

	const std::string GetName() { return m_sName; };
	const glm::vec3 GetPosition() { return m_v3Position; };
	const glm::vec3 GetScale() { return m_v3Scale; };
	const GLuint GetTextureID() { return m_uiTextureID; };
	const float GetFrameModifier();
	const float GetFrameRatio() { return m_fFrameRatio; };
	const glm::mat4 GetTransform() { return m_m4Transform; };
	
	const bool GetIfActive() { return m_bActive; };

	void ToggleActive() { m_bActive = !m_bActive; };
	void ToggleHighlighted() { m_bHighlighted = !m_bHighlighted; };


private:
	bool m_bActive = false;
	bool m_bHighlighted = false;
	unsigned int m_uiInactiveFrameIndex = 0;
	unsigned int m_uiActiveFrameIndex = 0;
	unsigned int m_uiNumberOfFrames = 0;
	float m_fFrameRatio = 0.0f;
	std::string m_sName;

	GLuint m_uiTextureID = 0;
	glm::vec3 m_v3Position;
	glm::vec2 m_v2MaxPoints;
	glm::vec2 m_v2MinPoints;
	glm::vec3 m_v3Scale;
	glm::mat4 m_m4Transform;

	CSquare& m_pSquareRef = CSquare::GetRef();
	CCamera& m_pCameraRef = CCamera::GetRef();
};

#endif // !___BUTTON_H___