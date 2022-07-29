
/******************************************************
Bachelor of Software Enginering
Media Design School
Auckland
New Zealand
(c)2021 Media Design School
File Name : CTextLabel
Description : Class for displaying text
Author : Chuang Sing, Kee
Mail : chuang.kee@mds.ac.nz
********************************************************/

#pragma once
#ifndef ___CTEXTLABEL_H__
#define ___CTEXTLABEL_H__

//Dependencies
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

//Library
#include <map>
#include <string>
#include <iostream>

//local
#include "ShaderLoader.h"
#include "Utility.h"
#include "Camera.h"

class CTextLabel
{
public:
	CTextLabel(
		std::string _inText,
		glm::ivec2 _inPixelSize,
		glm::vec2 _inPos,
		std::string _inFont = std::string(),
		glm::vec3 _inColour	= glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2 _inScale	= glm::vec2(1.0f,1.0f));

	~CTextLabel();

	void Render();
	void Update(float _inTime, float _inDeltaTime);
	void SetText(std::string _inText) {
		this->TextStr = _inText; 
		DetermineTextLengthHeight();
	}
	std::string GetText() { return this->TextStr; };

	void RemoveLastLetter()								{ if(!TextStr.empty()) TextStr.pop_back(); };
	void AddLetter(char _inChar)						{ TextStr.push_back(_inChar); };
	void ToggleEditMode();
	bool IsEditModeActive()								{ return bEditMode; };

	void SetColour(glm::vec3 _inColour)					{ this->Colour = _inColour; };
	void SetScale(glm::vec2 _inScale) {
		this->BaseScale = _inScale; 
		DetermineTextLengthHeight();
	};
	void SetPosition(glm::vec2 _inPosition)				{ this->Position = _inPosition; };
	void SetScaleBounceMultiplier(float _inMultiplier)	{ fScaleBounce_Multiplier = _inMultiplier; };
	void EnableScaleBounce()							{ bScaleBounce_Enabled = true; };
	//When disabling, need to make sure Scale is reset to its base
	void DisableScaleBounce();
	void EnableTextScrolling()							{ bScolling_Enabled = true; };
	void DisableTextScrolling()							{ bScolling_Enabled = false; ScrollOffset = 0; };
	void SwitchScrollDirecion()							{ bScrollToRight = !bScrollToRight; };
	void EnableTextBlinking()							{ bBlink_Enabled = true; };
	void DisableTextBlinking()							{ bBlink_Enabled = false; bIsOpaque = true; fBlinkCountUp = 0; };

private:
	struct FontChar
	{
		GLuint		TextureID = 0;
		glm::ivec2	Size;
		glm::ivec2	Bearing;
		GLuint		Advance = 0;
	};

	GLuint GenerateTexture(FT_Face _inFace);
	void DetermineTextLengthHeight();

	CCamera& pCameraRef = CCamera::GetRef();
	const char* m_pcVertex_Default = "Resources/Shaders/Text.vert";
	const char* m_pcFrag_Default = "Resources/Shaders/Text.frag";
	const char* m_pcfileLocation_font = "Resources/Fonts/ARIAL.TTF";

	const GLfloat Scroll_HideWidth = 32.0f;

	static const int kiFontCharacterLimit;
	static const float kfBlinkPeriod;
	static const float kfScrollSpeed;

	bool Initialized = false;
	bool bScaleBounce_Enabled = false;
	bool bScolling_Enabled = false;
	bool bScrollToRight = false;
	bool bBlink_Enabled = false;
	bool bEditMode = false;
	bool bIsOpaque = true;
	float fBlinkCountUp = 0;
	float fScaleBounce_Multiplier = 1.0f;

	std::string TextStr;
	glm::vec3 Colour;
	glm::vec2 Scale;
	glm::vec2 BaseScale;
	glm::vec2 Position;
	glm::vec2 PosOffset;
	glm::vec2 MaxSizes;
	GLuint VAO_Text = 0;
	GLuint EBO_Text = 0;
	GLuint VBO_DynamicQuad = 0;
	GLuint Program_Text = 0;
	GLfloat TextLength_Scrolling = 0;
	GLfloat ScrollOffset = 0.0f;
	glm::mat4 ProjectionMat;

	std::map<GLchar, FontChar> CharacterMap;

	const 	GLuint Indices[6] = {
		0, 1, 2,
		0, 2, 3,
	};
};

#endif // !___CTEXTLABEL_H__