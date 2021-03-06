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

#include "IntroState2.h"

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
CIntroState2::CIntroState2(void)
	: background(NULL)
	, cSoundController(NULL)
{

}

/**
 @brief Destructor
 */
CIntroState2::~CIntroState2(void)
{
}

/**
 @brief Init this class instance
 */
bool CIntroState2::Init(void)
{
	cout << "CIntroState2::Init()\n" << endl;

	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\1st.wav"), 40, true, false, false);

	// Include Shader Manager
	CShaderManager::GetInstance()->Use("2DShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	//Create Background Entity
	background = new CBackgroundEntity("Image/introbg2.jpg");
	background->SetShader("2DShader");
	background->Init();
	voiceover = false;
	return true;
}

/**
 @brief Update this class instance
 */
bool CIntroState2::Update(const double dElapsedTime)
{
	if (voiceover == false)
	{
		cSoundController->PlaySoundByID(40);
		cout << "Playing IntroState 2 Sound" << endl;
		voiceover = true;
	}

	//cout << "CIntroState2::Update()\n" << endl;
	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_SPACE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		cout << "Loading MenuState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("IntroState3");
		return true;
	}

	return true;
}

/**
 @brief Render this class instance
 */
void CIntroState2::Render()
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);

	//Draw the background
 	background->Render();
}

/**
 @brief Destroy this class instance
 */
void CIntroState2::Destroy(void)
{
	// Delete the background
	if (background)
	{
		delete background;
		background = NULL;
	}

	cout << "CIntroState2::Destroy()\n" << endl;
}