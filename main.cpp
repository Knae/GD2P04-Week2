//============
//Defines
//#define STB_IMAGE_IMPLEMENTATION
//==========
//Includes
#include <ctime>
#include <sstream>
#include "Camera.h"
#include "Input.h"
#include "TextLabel.h"
#include "ObjectManager.h"
//==========
//Singleton references
CObjectManager& rObjMgr = CObjectManager::GetRef();
CCamera& rCamera = CCamera::GetRef();
//Global pointers
GLFWwindow* pWindow = nullptr;
CTextLabel* pTextDisplayUsername_Bounce = nullptr;
CTextLabel* pTextDisplayUsername_Scroll = nullptr;
CTextLabel* pTextDisplay_MousePos = nullptr;
//Constant Variables
const int kWindowWidth = 800;
const int kWindowHeight = 800;
const glm::vec3 kCameraPos = glm::vec3(0.0f,0.0f,10.0f);
const std::string sCursorToggle = "CursorToggle";
const std::string sWireframeToggle = "WireframeToggle";
//Static variables
float sfPreviousTime = 0.0f;
float sfCurrentTime = 0.0f;
bool bShift_Pressed = false;
bool bCapsLock = false;
bool bWireFrame = false;
bool bShowMouse = false;
//Forward Declarations
bool SetupDisplay();
bool SetupObjects();
void Update();
void Draw();
void ParseKeyCallback(GLFWwindow* _inWindow, int _inKey, int _inScanCode, int _inAction, int _inMod);
void MouseButtonCallback(GLFWwindow* _inWindow, int _inButtonCode, int _inActionCode, int _inMod);
void CursorPosCallback(GLFWwindow* _inWindow, double _inPosX, double _inPosY);
void Cleanup();

int main()
{
	if (!SetupDisplay())
	{
		std::cout << "Failed at setup phase. Exiting\n";
		system("pause");
		return -1;
	}

	if (!SetupObjects())
	{
		std::cout << "Failed object setup\n";
		system("pause");
		return -1;
	}

	std::cout << "Starting program loop\n";
	//program loop
	while (glfwWindowShouldClose(pWindow) == false)
	{
		//Update what needs to be updated
		Update();
		//Render objects
		Draw();
	}

	Cleanup();
	return 1;
}

/// <summary>
/// Setup display window and relevant modifiers
/// </summary>
/// <returns></returns>
bool SetupDisplay()
{
	try
	{
		//So that any decimal values displayed are to 2 decimal points
		std::cout << std::fixed << std::setprecision(2);

		//rUtil.setWindowSize(kWindowWidth, kWindowHeight);

		glfwInit();
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		//Create a GLFW Window
		pWindow = glfwCreateWindow(	kiWindowWidth,
									kiWindowHeight,
									"Summative 2", NULL, NULL);

		if (pWindow == NULL)
		{
			std::cout << "ERROR: Unable to create window.\n GLFW failed to initialize.\n ABORT!" << std::endl;
			system("pause");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(pWindow);

		//Initialize GLEW
		if (glewInit() != GLEW_OK)
		{
			std::cout << "ERROR: GLEW initialization failed.\nABORT!" << std::endl;
			system("pause");
			glfwTerminate();
			return false;
		}

		//Set background colour to black
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glViewport(0, 0, 800, 800);
		//Depth Test Function
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//SetupCameraPos
		rCamera.SetPos(kCameraPos);
		//Setup Callbacks
		glfwSetKeyCallback(pWindow, ParseKeyCallback);
		glfwSetMouseButtonCallback(pWindow, MouseButtonCallback);
		glfwSetCursorPosCallback(pWindow, CursorPosCallback);
		//Hide Mouse
		glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		std::cout << "Window setup complete\n";

		sfPreviousTime = (float)glfwGetTime();

		std::cout << "Initial variables setup done\n";

		return true;
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to setup display window\n";
		return false;
	}
}

/// <summary>
/// Setup object meshes and positions of objects
/// </summary>
/// <returns></returns>
bool SetupObjects()
{
	try
	{
		rObjMgr.Initialize();
		//Setup Cubes
		//Rotating cube 1
		rObjMgr.CreateObject(	CObjectManager::ETYPE::CUBE,
								glm::vec3(-1.5f, 0.50f, 0.0f),
								glm::vec3(0.5f),
								false,
								1,
								glm::vec3(0.0f,1.0f,0.0f),
								false,
								glm::vec3(0.0f), 50.0f);
		//Rotating cube 2
		rObjMgr.CreateObject(	CObjectManager::ETYPE::CUBE,
								glm::vec3(2.0f, 0.50f, 0.0f),
								glm::vec3(0.5f),
								false,
								-1,
								glm::vec3(0.0f, 1.0f, 0.0f),
								false,
								glm::vec3(0.0f),
								25.0f);

		//Dark Red Controllable diamond
		rObjMgr.CreateObject(	CObjectManager::ETYPE::DIAMOND,
								glm::vec3(0.0f, 0.0f, 0.0f),
								glm::vec3(1.0f),
								true,
								1,
								glm::vec3(0.0f, 1.0f, 0.0f),
								true,
								glm::vec3(0.0f),
								10.0f);

		//Button - Cursor Toggle
		rObjMgr.CreateButton(	CObjectManager::EBUTTONTYPE::CURSOR,
								sCursorToggle,
								glm::vec3((-kfWindowWidth_Half * 0.75f), (kfWindowHeight_Half* 0.65f), 1.0f),
								glm::vec3(50.0f),
								4,
								0,
								2);

		//Button - Wireframe Toggle
		rObjMgr.CreateButton(CObjectManager::EBUTTONTYPE::WIRE,
								sWireframeToggle,
								glm::vec3((-kfWindowWidth_Half * 0.75f), (kfWindowHeight_Half * 0.45f), 1.0f),
								glm::vec3(50.0f),
								4,
								0,
								2);

		//UsernameText Bouncing
		pTextDisplayUsername_Bounce = new CTextLabel(	"Test Username",
														glm::ivec2(0,48),
														glm::vec2(400.0f,200.0f));
		pTextDisplayUsername_Bounce->SetScale(glm::vec2(0.5f));
		pTextDisplayUsername_Bounce->SetScaleBounceMultiplier(2.0f);
		pTextDisplayUsername_Bounce->EnableScaleBounce();

		//Scrolling UsernameText
		pTextDisplayUsername_Scroll = new CTextLabel(	"Test Username",
														glm::ivec2(0, 48),
														glm::vec2(400.0f, 700.0f));
		pTextDisplayUsername_Scroll->SetScale(glm::vec2(0.5f));
		pTextDisplayUsername_Scroll->EnableTextScrolling();
	
		//MousePos Text
		pTextDisplay_MousePos = new CTextLabel(	"X: 0000 Y:0000",
												glm::ivec2(0, 24),
												glm::vec2(100.0f, kiWindowHeight - 50.0f));
		pTextDisplay_MousePos->SetScale(glm::vec2(0.5f));

		std::cout << "Object setup complete\n";
		return true;
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to setup Objects\n";
		return false;
	}
}

/// <summary>
/// Update all the elements in the program
/// </summary>
void Update()
{
	glfwPollEvents();

	//Update camera matrices
	rCamera.Update();
	//calculate deltaTime
	sfCurrentTime = (float)glfwGetTime();
	float fDeltaTime = sfCurrentTime - sfPreviousTime;
	//Update Objects through manager
	rObjMgr.UpdateObjects(fDeltaTime);
	if (pTextDisplayUsername_Bounce != nullptr)
	{
		pTextDisplayUsername_Bounce->Update(sfCurrentTime, fDeltaTime);
	}

	if (pTextDisplayUsername_Scroll != nullptr)
	{
		pTextDisplayUsername_Scroll->Update(sfCurrentTime, fDeltaTime);
	}

	sfPreviousTime = sfCurrentTime;
}

/// <summary>
/// Render the elements, after setting relevant grapical settings where
/// necessary
/// </summary>
void Draw()
{
	//Clear back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (bWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (bShowMouse)
	{
		glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	rObjMgr.DrawObjects();

	if (pTextDisplayUsername_Bounce != nullptr)
	{
		pTextDisplayUsername_Bounce->Render();
	}
	if (pTextDisplayUsername_Scroll != nullptr)
	{
		pTextDisplayUsername_Scroll->Render();
	}
	if (pTextDisplay_MousePos != nullptr)
	{
		pTextDisplay_MousePos->Render();
	}
	glfwSwapBuffers(pWindow);
}

/// <summary>
/// Key press event. Parse what action to execute
/// based on key press
/// </summary>
/// <param name="_inWindow"></param>
/// <param name="_inKey"></param>
/// <param name="_inScanCode"></param>
/// <param name="_inAction"></param>
/// <param name="_inMod"></param>
void ParseKeyCallback(GLFWwindow* _inWindow, int _inKey, int _inScanCode, int _inAction, int _inMod)
{
	//ESC closese the program
	if (_inKey == GLFW_KEY_ESCAPE && _inAction == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(_inWindow, true);
	}

	//F toggles wire frame
	if (_inKey == GLFW_KEY_F && _inAction == GLFW_RELEASE)
	{ 
		rObjMgr.ToggleButtonActiveState(sWireframeToggle);
		bWireFrame = rObjMgr.GetIfButtonActive(sWireframeToggle);
	}

	//V toggles cursor display
	if (_inKey == GLFW_KEY_V && _inAction == GLFW_RELEASE)
	{
		rObjMgr.ToggleButtonActiveState(sCursorToggle);
		bShowMouse = rObjMgr.GetIfButtonActive(sCursorToggle);
	}

	//R switches scroll direction
	if (_inKey == GLFW_KEY_R && _inAction == GLFW_RELEASE)
	{
		pTextDisplayUsername_Scroll->SwitchScrollDirecion();
	}

	//SHIFT key for text input
	if (_inKey == GLFW_KEY_RIGHT_SHIFT || _inKey == GLFW_KEY_LEFT_SHIFT)
	{
		if (_inAction == GLFW_PRESS)
		{
			bShift_Pressed = true;
		}
		else
		{
			bShift_Pressed = false;
		}
	}

	//ENTER to begin text editing
	if (_inKey == GLFW_KEY_ENTER && _inAction == GLFW_RELEASE)
	{
		pTextDisplayUsername_Scroll->ToggleEditMode();
		if (!pTextDisplayUsername_Scroll->IsEditModeActive())
		{
			pTextDisplayUsername_Bounce->SetText(pTextDisplayUsername_Scroll->GetText());
		}
	}
	
	//This block handles the text being entered
	if (pTextDisplayUsername_Scroll->IsEditModeActive()	&& _inAction == GLFW_RELEASE 
		&& _inKey != GLFW_KEY_ENTER && _inKey != GLFW_KEY_RIGHT_SHIFT && _inKey != GLFW_KEY_LEFT_SHIFT)
	{
		if (_inKey == GLFW_KEY_BACKSPACE)
		{
			pTextDisplayUsername_Scroll->RemoveLastLetter();
		}
		else if ( _inKey == GLFW_KEY_CAPS_LOCK )
		{
			bCapsLock = !bCapsLock;
		}
		else
		{
			char inputChar = ParseButtonPressToChar(_inKey, (bShift_Pressed != bCapsLock));
			pTextDisplayUsername_Scroll->AddLetter(inputChar);
		}
	}

	//Movement keys to move the rainbow diamond
	if (_inKey == GLFW_KEY_W || _inKey == GLFW_KEY_S ||
		_inKey == GLFW_KEY_A || _inKey == GLFW_KEY_D ||
		_inKey == GLFW_KEY_Q || _inKey == GLFW_KEY_E)
	{
		rObjMgr.MoveControlledObjects(ProcessMovement(_inKey, _inAction));
	}
}

/// <summary>
/// mouse button event. Parse action to execute based
/// on mouse button press
/// </summary>
/// <param name="_inWindow"></param>
/// <param name="_inButtonCode"></param>
/// <param name="_inActionCode"></param>
/// <param name="_inMod"></param>
void MouseButtonCallback(GLFWwindow* _inWindow, int _inButtonCode, int _inActionCode, int _inMod)
{
	double xPos = 0.0f;
	double yPos = 0.0f;
	double height = kiWindowHeight;

	glfwGetCursorPos(_inWindow, &xPos, &yPos);

	if (_inButtonCode == GLFW_MOUSE_BUTTON_LEFT && _inActionCode == GLFW_RELEASE)
	{
		CObjectManager::EBUTTONTYPE buttonPressed = rObjMgr.CheckClickOnButtons(xPos, yPos);
		switch (buttonPressed)
		{
		case CObjectManager::EBUTTONTYPE::CURSOR:
		{
			bShowMouse = rObjMgr.GetIfButtonActive(sCursorToggle);
			break;
		}
		case CObjectManager::EBUTTONTYPE::WIRE:
		{
			bWireFrame = rObjMgr.GetIfButtonActive(sWireframeToggle);
			break;
		}
		case CObjectManager::EBUTTONTYPE::NONE:

		default:
		{
			break;
		}
		}
		std::cout << "event occured at x:" << xPos << " y:" << yPos << "\n\n";
	}
}

/// <summary>
/// Mouse move event. mainly for displaying mouse position
/// and checking is mouse is over buttons
/// </summary>
/// <param name="_inWindow"></param>
/// <param name="_inPosX"></param>
/// <param name="_inPosY"></param>
void CursorPosCallback(GLFWwindow* _inWindow, double _inPosX, double _inPosY)
{
	if (pTextDisplay_MousePos != nullptr)
	{
		std::stringstream shortenDecimal_X;
		std::stringstream shortenDecimal_Y;

		shortenDecimal_X << std::setprecision(0) << std::fixed << _inPosX;
		shortenDecimal_Y << std::setprecision(0) << std::fixed << _inPosY;
		std::string mousePosTxt = "X: " + shortenDecimal_X.str() + " Y: " + shortenDecimal_Y.str();
		pTextDisplay_MousePos->SetText(mousePosTxt);
	}

	rObjMgr.CheckIfMouseInButtons(_inPosX, _inPosY);
}

/// <summary>
/// Cleanup
/// </summary>
void Cleanup()
{
	delete pTextDisplayUsername_Bounce;
	delete pTextDisplayUsername_Scroll;
	delete pTextDisplay_MousePos;

	pTextDisplayUsername_Bounce = nullptr;
	pTextDisplayUsername_Scroll = nullptr;
	pTextDisplay_MousePos = nullptr;
}
