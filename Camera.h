
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CCamera
Description : singleton class containing functions and variables
related to the camera
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#ifndef ___CCAMERA_H___
#define ___CCAMERA_H___
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

/// <summary>
/// Camera class to store relevant camera variable, for example camera position and look direction
/// and also calculate relevant matrices for use with displaying objects in OpenGL
/// </summary>

class CUtility;

class CCamera
{
public:
	static CCamera& GetRef() 
	{
		if (m_thisCamera == nullptr)
		{
			m_thisCamera = new CCamera();
		}
		return *m_thisCamera;
	}

	void SetUpCamera(	glm::vec3 _inMousePos = glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3 _inPos		= glm::vec3(0.0f, 0.0f, 1.0f),
						glm::vec3 _inTargetPos	= glm::vec3(0.0f, 0.0f, 0.0f));

	void SetPos(glm::vec3 _inMatrix)		{ m_v3CameraPos = _inMatrix; }
	void SetTargetPos(glm::vec3 _inMatrix)	{ m_v3CameraTargetPos = _inMatrix; }
	void SetStartMousePos(glm::vec3 _inPos) { m_v3LastMousePos = _inPos; }
	void SetLookDir() { m_v3CameraLookDir = glm::normalize(m_v3CameraTargetPos - m_v3CameraPos); }
	void SetRightDir() { m_v3CameraRightDir = glm::normalize(glm::cross(m_kv3UpDir_World,m_v3CameraLookDir)); }
	void SetUpDir() { m_v3CameraUpDir = glm::normalize(glm::cross(m_v3CameraLookDir, m_v3CameraRightDir)); }
	void RotateCamera(double _inPosX, double _inPosY);
	void Update();
	glm::mat4 GetCameraMatrix_Proj()	{return m_m4CameraMatrix_Proj;}
	glm::mat4 GetCameraMatrix_Persp()	{return m_m4CameraMatrix_Persp;}

private:
	CCamera();
	~CCamera();
	CCamera(const CCamera&) = delete;

	CUtility* m_UtiltiyRef;
	static CCamera* m_thisCamera;

	float m_fLookYaw = -90.0f;
	float m_fLookPitch = 0.0f;

	glm::vec3 m_v3CameraPos;
	glm::vec3 m_v3CameraLookDir;
	glm::vec3 m_v3CameraTargetPos;
	glm::vec3 m_v3CameraUpDir;
	glm::vec3 m_v3CameraRightDir;
	glm::vec3 m_v3LastMousePos;
	glm::mat4 m_m4MatrixView			= glm::mat4();
	glm::mat4 m_m4MatrixProj			= glm::mat4();
	glm::mat4 m_m4MatrixPersp			= glm::mat4();
	glm::mat4 m_m4CameraMatrix_Proj		= glm::mat4();
	glm::mat4 m_m4CameraMatrix_Persp	= glm::mat4();

	const glm::vec3 m_kv3UpDir_World = glm::vec3(0.0f, 1.0f, 0.0f);
	const float m_kfMouseSensitivity = 2.0f;
	const float m_kMaxPitch = 85.0f;
	const float m_kMinPitch = -85.0f;
};
#endif // !___CCAMERA_H___