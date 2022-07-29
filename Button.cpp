#include "Button.h"

/// <summary>
/// Constructor, taking in the button name, position, size/scale, 
/// number of frames in the texture, which frame is active, which is inactive
/// and the textureID asscoiated with the texture used by the button
/// </summary>
/// <param name="_inName"></param>
/// <param name="_inPosition"></param>
/// <param name="_inScale"></param>
/// <param name="_inNumberOfFrames"></param>
/// <param name="_inInactiveFrame"></param>
/// <param name="_inActiveFrame"></param>
/// <param name="_inTextureID"></param>
CButton::CButton(	std::string _inName, glm::vec3 _inPosition, glm::vec3 _inScale,
					unsigned int _inNumberOfFrames, unsigned int _inInactiveFrame,
					unsigned int _inActiveFrame, GLuint _inTextureID )
{
	m_sName = _inName;
	m_v3Position = _inPosition;
	m_v3Scale = _inScale;
	m_uiNumberOfFrames = _inNumberOfFrames;
	m_uiInactiveFrameIndex = _inInactiveFrame;
	m_uiActiveFrameIndex = _inActiveFrame;
	m_uiTextureID = _inTextureID;
	
	m_fFrameRatio = 1 / (float)(m_uiNumberOfFrames);
	m_v2MaxPoints = glm::vec2(_inPosition.x + _inScale.x/2 ,_inPosition.y + _inScale.y/2);
	m_v2MinPoints = glm::vec2(_inPosition.x - _inScale.x/2 ,_inPosition.y - _inScale.y/2);
}

CButton::~CButton()
{
}

/// <summary>
/// Update the transform matrix, in the event the camera moves
/// </summary>
/// <param name="_inDeltaTime"></param>
void CButton::Update(float _inDeltaTime)
{
	glm::mat4 matrixRotate = glm::mat4();
	glm::mat4 matrixTranslate = matrixTranslate = glm::translate(glm::mat4(), m_v3Position);
	glm::mat4 matrixScale = glm::scale(glm::mat4(),m_v3Scale);
	m_m4Transform = m_pCameraRef.GetCameraMatrix_Proj() * matrixTranslate * matrixRotate * matrixScale;
}

/// <summary>
/// Render, just calling the draw function in the square
/// </summary>
void CButton::Render()
{
	m_pSquareRef.Draw();
}

/// <summary>
/// Check if the coodinates provided is within the boundary of
/// the button
/// </summary>
/// <param name="_inPosX"></param>
/// <param name="_inPosY"></param>
/// <returns></returns>
bool CButton::CheckIfMouseWithinButton(double _inPosX, double _inPosY)
{
	double mouseX = _inPosX - kfWindowWidth_Half;
 	double mouseY = kfWindowWidth_Half - _inPosY;
	if (	(mouseX < m_v2MaxPoints.x && mouseX > m_v2MinPoints.x) &&
			(mouseY < m_v2MaxPoints.y && mouseY > m_v2MinPoints.y))
	{
		//std::cout << "Mouse is in the button!\n";
		m_bHighlighted = true;
		return true;
	}
	else
	{
		m_bHighlighted = false;
		return false;
	}
}

/// <summary>
/// return the frame modifier, which will determine
/// which part to the texture file to use
/// </summary>
/// <returns></returns>
const float CButton::GetFrameModifier()
{	//0 = Inactive, not highligted; 1 = Inactive, highlighted
	//2 = Active, not hightlighted; 3 = Active, highlighted
	int currentFrame = (m_bActive ? 2 : 0) + (m_bHighlighted? 1 : 0);
	return m_fFrameRatio * currentFrame;
}
