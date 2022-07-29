//============
//Defines
#define STB_IMAGE_IMPLEMENTATION

#include "ObjectManager.h"
#include <stb_image.h>

CObjectManager*  CObjectManager::m_thisManager = nullptr;

void CObjectManager::Initialize()
{
	//Initialize all shape meshes
	m_pCubeMeshRef.SetupShape();
	m_pDiamondRef.SetupShape();
	m_pSquareRef.SetupShape();

	SetupPrograms();
	SetupTextures();
}

/// <summary>
/// Create an object. Object settings can't be changed after creation
/// </summary>
/// <param name="_inType"></param>
/// <param name="_inPos"></param>
/// <param name="_inScale"></param>
/// <param name="_inProgramID"></param>
/// <param name="_inRotateDir"></param>
/// <param name="_inRotateAxis"></param>
/// <param name="_inRotateInPlace"></param>
/// <param name="_inRotatePoint"></param>
void CObjectManager::CreateObject(	ETYPE _inType,
 									glm::vec3 _inPos,
									glm::vec3 _inScale,
									bool _inIsControlled = false,
									short _inRotateDir = 0,
									glm::vec3 _inRotateAxis = glm::vec3(0.0f,0.0f,1.0f),
									bool _inRotateInPlace = true,
									glm::vec3 _inRotatePoint = glm::vec3(0.0f,0.0f,1.0f), 
									float _inRotateSpeed = 0.0f)
{
	TObject* newObject = new TObject();
	newObject->eType = _inType;
	newObject->v3Position = _inPos;
	newObject->v3Scale = _inScale;
	newObject->siRotateDir = _inRotateDir;
	newObject->v3RotateAxis = _inRotateAxis;
	newObject->bRotateInPlace = _inRotateInPlace;
	newObject->v3RotatePoint = _inRotatePoint;
	newObject->fRotationSpeed = _inRotateSpeed;
	newObject->bIsControlled = _inIsControlled;

	//Stores programID based on the shape type
	switch (_inType)
	{
	case CObjectManager::ETYPE::NONE:
	{
		break;
	}
	case CObjectManager::ETYPE::CUBE:
	{
		newObject->uiProgramID = uiProgramID_Cube_Rotating_Origin;
		break;
	}
	case CObjectManager::ETYPE::DIAMOND:
	{
		newObject->uiProgramID = uiProgramID_Diamond_Controlled;
		break;
	}
	case CObjectManager::ETYPE::MAXTYPE:
	{
		break;
	}
	default:
		break;
	}

	m_vecObjects.push_back(newObject);
	newObject = nullptr;
}

/// <summary>
/// Create a button, pass the textureID associated with the button type as 
/// well as the other relevant inputs to the button class constructor
/// </summary>
/// <param name="_inType"></param>
/// <param name="_inName"></param>
/// <param name="_inPosition"></param>
/// <param name="_inScale"></param>
/// <param name="_inNumberOfFrames"></param>
/// <param name="_inInactiveFrame"></param>
/// <param name="_inActiveFrame"></param>
void CObjectManager::CreateButton(	EBUTTONTYPE _inType, std::string _inName, glm::vec3 _inPosition,
									glm::vec3 _inScale ,unsigned int _inNumberOfFrames, 
									unsigned int _inInactiveFrame, unsigned int _inActiveFrame)
{
	GLuint TextureID = _inType == EBUTTONTYPE::CURSOR ? m_TextureIDCursorToggle : m_TextureIDWireframeToggle;
	CButton* newButton = new CButton(_inName, _inPosition, _inScale, _inNumberOfFrames, _inInactiveFrame, _inActiveFrame, TextureID);
	m_vecButtons.push_back(newButton);
	newButton = nullptr;
}

/// <summary>
/// recalculate the tranform matrix for each object and button created
/// </summary>
/// <param name="_inDeltaTime"></param>
void CObjectManager::UpdateObjects(float _inDeltaTime)
{
	for (auto element:m_vecObjects)
	{
		glm::mat4 matrixRotate = glm::mat4();
		glm::mat4 matrixRotateAroundPoint = glm::mat4();
		glm::mat4 matrixTranslate = glm::mat4();
		glm::mat4 matrixScale = glm::scale(glm::mat4(), element->v3Scale);

		float fRotateAmount = _inDeltaTime * (element->fRotationSpeed);

		//if the movevector is not zero, then change the object position based on it.
		if (glm::length(element->v3MoveVector) != 0)
		{
			glm::vec3 finalLocation = (glm::normalize(element->v3MoveVector)) * m_fMoveSpeed * _inDeltaTime;
 			element->v3Position += finalLocation;
			matrixTranslate = glm::translate(glm::mat4(), element->v3Position);
		}
		else
		{
			matrixTranslate = glm::translate(glm::mat4(),element->v3Position);
		}

		if (element->siRotateDir > 0)
		{
			if ((element->fCurrentRotateAngle += fRotateAmount) >= 360.0f)
			{
				element->fCurrentRotateAngle -= 360.0f;
			}
		}
		else if (element->siRotateDir < 0)
		{
			if ((element->fCurrentRotateAngle -= fRotateAmount) <= 0.0f)
			{
				element->fCurrentRotateAngle += 360.0f;
			}
		}

		if (element->bRotateInPlace)
		{
			if (element->siRotateDir != 0)
			{
				matrixRotate = glm::rotate(glm::mat4(), glm::radians(element->fCurrentRotateAngle), element->v3RotateAxis);
			}
			else
			{
				//use identity matrix for rotation
				matrixRotate = glm::mat4();
			}
		}
		else
		{
			//planned to allow rotation around any point, not just world origin
			//Possible method is to translate result so that rotation point is at world origin,
			//  apply rotation then translate back
			//so T(-rPT)*R()*T(rPT)*result where rPT-rotation point, R() - rotation matrix, T() - translate matrix
			if (element->siRotateDir != 0)
			{
				matrixRotateAroundPoint =
					glm::rotate(glm::mat4(), glm::radians(element->fCurrentRotateAngle), element->v3RotateAxis);
			}
			else
			{
				//Use identity matrix for rotation
				matrixRotate = glm::mat4();
			}
		}
 		element->m4Transform = m_pCameraRef.GetCameraMatrix_Persp() * matrixRotateAroundPoint * matrixTranslate * matrixRotate * matrixScale;
	}

	for (auto element : m_vecButtons)
	{
		element->Update(_inDeltaTime);
	}
}

/// <summary>
/// Takes in a move vector and adds it to the movevector of each
/// object marked as controlled by user
/// </summary>
/// <param name="_inMoveVector"></param>
void CObjectManager::MoveControlledObjects(glm::vec3 _inMoveVector)
{
	for (auto element:m_vecObjects)
	{
		if (element->bIsControlled)
		{
			element->v3MoveVector += _inMoveVector;
		}
	}
}

/// <summary>
/// Takes in the mouse position and calls the check function in each button
/// to update its states if necessary
/// </summary>
/// <param name="_inMouseX"></param>
/// <param name="_inMouseY"></param>
void CObjectManager::CheckIfMouseInButtons(double _inMouseX, double _inMouseY)
{
	for (auto element : m_vecButtons)
	{
		element->CheckIfMouseWithinButton(_inMouseX, _inMouseY);
	}
}

/// <summary>
/// Should be called when a left click is detected.
/// determines if the mouse was within a button boundary
/// then returns the button type. Returns NONE if
/// it wasn't within a button boundary
/// </summary>
/// <param name="_inMouseX"></param>
/// <param name="_inMouseY"></param>
/// <returns></returns>
CObjectManager::EBUTTONTYPE CObjectManager::CheckClickOnButtons(double _inMouseX, double _inMouseY)
{
	for (auto element : m_vecButtons)
	{
		if (element->CheckIfMouseWithinButton(_inMouseX,_inMouseY))
		{
			element->ToggleActive();
			if (strcmp(element->GetName().c_str(), "CursorToggle") == 0)
			{
				return EBUTTONTYPE::CURSOR;
			}
			else if (strcmp(element->GetName().c_str(), "WireframeToggle") == 0)
			{
				return EBUTTONTYPE::WIRE;
			}
		}
	}
	return EBUTTONTYPE::NONE;
}

/// <summary>
/// Finds the button with the same _inName then returns true if 
/// the button is active. else it returns false
/// </summary>
/// <param name="_inName"></param>
/// <returns></returns>
const bool CObjectManager::GetIfButtonActive(std::string _inName)
{
	for (auto element:m_vecButtons)
	{
		if (strcmp(element->GetName().c_str(), _inName.c_str()) == 0)
		{
			return element->GetIfActive();
		}
	};
	return false;
}

/// <summary>
/// Finds the button with the corresponding name then toggles
/// its active state
/// </summary>
/// <param name="_inName"></param>
void CObjectManager::ToggleButtonActiveState(std::string _inName)
{
	for (auto element : m_vecButtons)
	{
		if (strcmp(element->GetName().c_str(), _inName.c_str()) == 0)
		{
			element->ToggleActive();
		}
	};
}

/// <summary>
/// Renders each object, assuming they contain uniforms for 
/// a texture input and a matrix input.
/// assume main class has cleared back buffer
/// </summary>
void CObjectManager::DrawObjects()
{
	GLint TransformMatLoc = 0;
	for (auto element : m_vecObjects)
	{
		switch (element->eType)
		{
			case ETYPE::CUBE:
			{
				glUseProgram(uiProgramID_Cube_Rotating_Origin);
				//Bind given texture ID
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_TextureIDLaughingMan);
				glUniform1i(glGetUniformLocation(uiProgramID_Cube_Rotating_Origin, "ImageTexture1"), 0);
				//Pass matrices to shader
				GLint TransformMatrixLoc = glGetUniformLocation(uiProgramID_Cube_Rotating_Origin, "inputMatrix");
				glUniformMatrix4fv(TransformMatrixLoc, 1, GL_FALSE, glm::value_ptr(element->m4Transform));
				m_pCubeMeshRef.Draw();
				glBindTexture(GL_TEXTURE_2D, 0);
				glUseProgram(0);
				break;
			}
			case ETYPE::DIAMOND:
			{
				glUseProgram(uiProgramID_Diamond_Controlled);
				////Pass colour to shader
				//glUniform3fv(glGetUniformLocation(uiProgramID_Diamond_Controlled, "Colour"), 1, glm::value_ptr(element->v3Colour));
				//Pass matrices to shader
				GLint TransformMatrixLoc = glGetUniformLocation(uiProgramID_Diamond_Controlled, "inputMatrix");
				glUniformMatrix4fv(TransformMatrixLoc, 1, GL_FALSE, glm::value_ptr(element->m4Transform));
				m_pDiamondRef.Draw();
				glUseProgram(0);
				break;
			}
			default:
			{
				std::cout << "ERROR: Attempted to draw unknown shape. ShapeCode: " << (short)element->eType << "\n";
				break;
			}
		}
	}

	for (auto element : m_vecButtons)
	{
		glUseProgram(uiProgramID_Button);
		//Bind given texture ID
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, element->GetTextureID());
		glUniform1i(glGetUniformLocation(uiProgramID_Button, "ImageTexture1"), 0);
		//Pass uniforms to shader
		float frameModifier = element->GetFrameModifier() +1;
		float frameRatio = element->GetFrameRatio();
		glm::mat4 transform = element->GetTransform();
		glUniform2f(glGetUniformLocation(uiProgramID_Button, "FrameModifier"), frameModifier, 0.0f);
		glUniform1f(glGetUniformLocation(uiProgramID_Button,"FrameRatio"), frameRatio);
		//Pass matrices to shader
		GLint TransformMatrixLoc = glGetUniformLocation(uiProgramID_Button, "inputMatrix");
		glUniformMatrix4fv(TransformMatrixLoc, 1, GL_FALSE, glm::value_ptr(transform));
		element->Render();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
}

CObjectManager::CObjectManager()
{
}

/// <summary>
/// Clears all the vectors before 
/// clearing its own pointer
/// </summary>
CObjectManager::~CObjectManager()
{
	for (auto element : m_vecObjects)
	{
		delete element;
		element = nullptr;
	}

	for (auto element : m_vecButtons)
	{
		delete element;
		element = nullptr;
	}

	m_vecButtons.clear();
	m_vecObjects.clear();

	delete m_thisManager;
	m_thisManager = nullptr;
}

/// <summary>
/// Setup textures that will be used by the program
/// and setup relevant graphical settings
/// </summary>
void CObjectManager::SetupTextures()
{
	try
	{
		//Load from file
		stbi_set_flip_vertically_on_load(true);
		m_TextureIDLaughingMan = LoadImage(m_kcFileLocation_LaughingMan);
		m_TextureIDCursorToggle = LoadImage(m_kcFileLocation_CursorToggle);
		m_TextureIDWireframeToggle = LoadImage(m_kcFileLocation_WireframeToggle);

		//Enable Culling
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to setup Textures\n";
	}
}

/// <summary>
/// Setup programs to be used by objects and buttons
/// </summary>
void CObjectManager::SetupPrograms()
{
	uiProgramID_Cube_Rotating_Origin = ShaderLoader::CreateProgram(m_kcFileLocation_TexturedVertex, m_kcFileLocation_OneTextureFrag);
	uiProgramID_Diamond_Controlled = ShaderLoader::CreateProgram(m_kcFileLocation_ColouredBaseVertex, m_kcFileLocation_BaseFrag);
	uiProgramID_Button = ShaderLoader::CreateProgram(m_kcFileLocation_ButtonVertex, m_kcFileLocation_OneTextureFrag);
}

/// <summary>
/// Load image into gpu memory, return Texture ID
/// </summary>
/// <param name="_inLocation"></param>
/// <returns></returns>
GLuint CObjectManager::LoadImage(const char* _inLocation)
{
	GLuint IDTexture = 0;
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	//=====================================================================================================
	//Load ImageData
	unsigned char* ImageData = stbi_load(_inLocation, &ImageWidth, &ImageHeight, &ImageComponents, 0);
	//Check how many image components and pass enum to golabl variable
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;
	//=====================================================================================================
	//=====================================================================================================
	//SetupTextures
	glGenTextures(1, &IDTexture);
	glBindTexture(GL_TEXTURE_2D, IDTexture);
	//Populate texture with image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
		LoadedComponents, GL_UNSIGNED_BYTE, ImageData);
	//Generate mipmaps, then free memory and unbind texture
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, black);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return IDTexture;
}
