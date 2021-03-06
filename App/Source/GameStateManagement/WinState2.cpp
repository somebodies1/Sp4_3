// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "WinState2.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include Mesh Builder
#include "Primitives/MeshBuilder.h"
// Include ImageLoader
#include "System\ImageLoader.h"
// Include Shader Manager
#include "RenderControl\ShaderManager.h"

 // Include shader
#include "RenderControl\shader.h"

// Include CSettings
#include "GameControl/Settings.h"

// Include CKeyboardController
#include "Inputs/KeyboardController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CWinState2::CWinState2(void)
	: background(NULL)
	, cSoundController(NULL)
{

}

/**
 @brief Destructor
 */
CWinState2::~CWinState2(void)
{
}

/**
 @brief Init this class instance
 */
bool CWinState2::Init(void)
{
	cout << "CWinState2::Init()\n" << endl;

	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\3rd.wav"), 42, true, false, false);

	// Include Shader Manager
	CShaderManager::GetInstance()->Use("2DShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	//Create Background Entity
	background = new CBackgroundEntity("Image/GUI/winscreen22.jpg");
	background->SetShader("2DShader");
	background->Init();

	voiceover = false;

	return true;
}

/**
 @brief Update this class instance
 */
bool CWinState2::Update(const double dElapsedTime)
{
	if (voiceover == false)
	{
		cSoundController->PlaySoundByID(42);
		cout << "Playing WinState 2 Sound" << endl;
		voiceover = true;
	}

	//cout << "CWinState2::Update()\n" << endl;
	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_SPACE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		cout << "Loading MenuState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("WinState3");
		return true;
	}

	return true;
}

/**
 @brief Render this class instance
 */
void CWinState2::Render()
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);

	//Draw the background
 	background->Render();
}

/**
 @brief Destroy this class instance
 */
void CWinState2::Destroy(void)
{
	// Delete the background
	if (background)
	{
		delete background;
		background = NULL;
	}

	cout << "CWinState2::Destroy()\n" << endl;
}