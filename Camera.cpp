#include "Camera.h"
#include "Utility.h"

CCamera* CCamera::m_thisCamera = nullptr;

/// <summary>
/// Rotate camera based on mouse position 
/// </summary>
/// <param name="_inPosX"></param>
/// <param name="_inPosY"></param>
void CCamera::RotateCamera(double _inPosX, double _inPosY)
{
   float Offset_X = (float) (_inPosX - m_v3LastMousePos.x) * m_kfMouseSensitivity;
   float Offset_Y = (float) (_inPosY - m_v3LastMousePos.y) * m_kfMouseSensitivity;

   m_v3LastMousePos = glm::vec3(_inPosX, _inPosY, 0.0f);

   m_fLookYaw += Offset_X;
   m_fLookPitch += Offset_Y;

   if (m_fLookPitch > m_kMaxPitch)
   {
	   m_fLookPitch = m_kMaxPitch;
   }
   else if (m_fLookPitch < m_kMinPitch)
   {
	   m_fLookPitch = m_kMinPitch;
   }

   glm::vec3 newDirection;
   newDirection.x = cos(glm::radians(m_fLookYaw)) * cos(glm::radians(m_fLookPitch));
   newDirection.y = sin(glm::radians(m_fLookPitch));
   newDirection.z = sin(glm::radians(m_fLookYaw)) * cos(glm::radians(m_fLookPitch));
   m_v3CameraLookDir = glm::normalize(newDirection);
}

/// <summary>
/// Constructor just calls the SetUpCamera function
/// </summary>
CCamera::CCamera()
{
	SetUpCamera();
}

/// <summary>
/// SetUp camera with initial values
/// </summary>
/// <param name="_inPos"></param>
/// <param name="_inLookDir"></param>
/// <param name="_inTargetPos"></param>
/// <param name="_inUpDir"></param>
void CCamera::SetUpCamera(glm::vec3 _inMousePos, glm::vec3 _inPos, glm::vec3 _inTargetPos)
{
	//m_UtiltiyRef = &CUtility::GetRef();

	SetPos(_inPos);
	SetTargetPos(_inTargetPos);
	SetStartMousePos(_inMousePos);

	//Calculate the projection matrix
	m_m4MatrixProj = glm::ortho(-(	kfWindowWidth_Half), 
									kfWindowWidth_Half,
								-(	kfWindowHeight_Half),
									kfWindowHeight_Half,
									0.1f, 100.0f);
	//calculate the perspective matrix
	m_m4MatrixPersp = glm::perspective(	glm::radians(45.0f),
										(float)kiWindowWidth / (float)kiWindowHeight,
										0.1f, 100.0f);
}


/// <summary>
/// Update the different directions and view matrices
/// </summary>
void CCamera::Update()
{
	//Update camera directions
	SetLookDir();
	SetRightDir();
	SetUpDir();
	//calculate the view matrix
	m_m4MatrixView = glm::lookAt(m_v3CameraPos, m_v3CameraPos + m_v3CameraLookDir, m_v3CameraUpDir);

	m_m4CameraMatrix_Proj = m_m4MatrixProj * m_m4MatrixView;
	m_m4CameraMatrix_Persp = m_m4MatrixPersp * m_m4MatrixView;
};


CCamera::~CCamera()
{
	delete m_thisCamera;
	m_thisCamera = nullptr;
}

