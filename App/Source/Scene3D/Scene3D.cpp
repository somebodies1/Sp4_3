/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene3D.h"

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif
//testttttttttttttttttttttttttttt
//Banana
//Julius Strong
//XL STRONG
// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include filesystem to read from a file
#include "System\filesystem.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

//Include CHutConcrete
#include "Entities/Hut_Concrete.h"
//Include SUV
#include "Entities/SUV.h"
//Include
#include "Entities/Dinosaur.h"
#include "Entities/Baryonyx.h"
#include "Entities/Charano.h"
#include "Entities/Edo.h"

// Include CTreeKabak3D
#include "Entities/TreeKabak3D.h"

//Include Math
#include "Math/MyMath.h"

// Include CSpinTower
#include "SceneGraph/SpinTower.h"


#include "SceneGraph/Wheel.h"
#include "SceneGraph/LVTower.h"
#include "SceneGraph/Planet.h"
#include "SceneGraph/Decor.h"

#include <iostream>
#include "Entities/Car.h"
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cSoundController(NULL)
	, cCamera(NULL)
	, cSkyBox(NULL)
	, cTerrain(NULL)
	, cSolidObjectManager(NULL)
	, cPlayer3D(NULL)
	, cProjectileManager(NULL)
	, cGUI_Scene3D(NULL)
	, cEntityManager(NULL)
	, cGameManager3D(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cEntityManager
	if (cEntityManager)
	{
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}

	// Destroy the cSoundController
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	// Destroy the cSkyBox
	if (cSkyBox)
	{
		cSkyBox->Destroy();
		cSkyBox = NULL;
	}

	// Destroy the cTerrain
	if (cTerrain)
	{
		cTerrain->Destroy();
		cTerrain = NULL;
	}

	// Destroy the cPlayer3D
	if (cPlayer3D)
	{
		cSolidObjectManager->Erase(cPlayer3D);
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	// Destroy the cSolidObjectManager
	if (cSolidObjectManager)
	{
		cSolidObjectManager->Destroy();
		cSolidObjectManager = NULL;
	}

	// Destroy the cProjectileManager
	if (cProjectileManager)
	{
		cProjectileManager->Destroy();
		cProjectileManager = NULL;
	}

	// Destroy the cGUI_Scene3D
	if (cGUI_Scene3D)
	{
		cGUI_Scene3D->Destroy();
		cGUI_Scene3D = NULL;
	}

	// Destroy the cEntityManager
	if (cGameManager3D)
	{
		cGameManager3D->Destroy();
		cGameManager3D = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	
	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	cSettings = CSettings::GetInstance();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	//cSoundController->LoadSound(FileSystem::getPath("Sounds\\bgmusic.mp3"), 1, true);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	//cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\bgmusic.ogg"), 4, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\reload.ogg"), 5, true, false, false);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\running.ogg"), 6, true, false, false);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\slide.ogg"), 7, true, false, false);
	//cSoundController->LoadSound(FileSystem::getPath("Sounds\\pistol.wav"), 7, true, false,true);

	cSoundController->PlaySoundByID(4);

	//cSoundController->PlaySoundByID(4);
	//Load Environment Entities
	//Load SkyBox
	cSkyBox = CSkyBox::GetInstance();
	//Set a shader to this class instance of CSkyBox
	cSkyBox->SetShader("Shader3D_SkyBox");
	cSkyBox->Init();

	//Load the ground
	cTerrain = CTerrain::GetInstance();
	cTerrain->SetShader("Shader3D_Terrain");
	cTerrain->Init();
	//Set size of Terrain
	cTerrain->SetRenderSize(100.0f, 5.0f, 100.0f);

	//Load movable Entities
	//Initialise CSolidObjectManager
	cSolidObjectManager = CSolidObjectManager::GetInstance();
	cSolidObjectManager->Init();

	//Initialise cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	cPlayer3D->SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	cPlayer3D->SetShader("Shader3D");
	cPlayer3D->Init();
	cPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	cPlayer3D->AttachCamera(cCamera);

	//Add cPlayer3D to cSolidObjectManager
	cSolidObjectManager->Add(cPlayer3D);

	//Initialise the projectile manager
	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");

	////Assign cPistol to cPlayer3D
	//CPistol* cPistol = new CPistol();
	////Set the position, rotation and scale of this weapon
	//cPistol->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	//cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	//cPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	////Initialise instance
	//cPistol->Init();
	//cPistol->SetShader("Shader3D_Model");
	//cPlayer3D->SetWeapon(0, cPistol);
	

	//PLAYER WEAPONS

	//Assign cCard to cPlayer3D
	CCard* cCard = new CCard();
	//Set the position, rotation and scale of this weapon
	cCard->SetPosition(glm::vec3(0.1f, -0.075f, -0.5f));
	cCard->SetRotation(3.14159f, glm::vec3(1.0f, 1.0f, 0.0f));
	cCard->SetScale(glm::vec3(0.1f, 0.2f, 0.1f));
	//Initialise instance
	cCard->Init();
	cCard->SetShader("Shader3D_Model");
	cPlayer3D->SetWeapon(0, cCard);

	////Assign cPistol to cPlayer3D
	//CPistol* cPistol = new CPistol();
	////Set the position, rotation and scale of this weapon
	//cPistol->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	//cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	//cPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	////Initialise instance
	//cPistol->Init();
	//cPistol->SetShader("Shader3D_Model");
	//cPlayer3D->SetWeapon(1, cPistol);

	//Assign cARifle to cPlayer3D
	CAssaultRifle* cARifle = new CAssaultRifle();
	//Set the position, rotation and scale of this weapon
	cARifle->SetPosition(glm::vec3(0.3f, -0.1f, -0.9f));
	//cARifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cARifle->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise instance
	cARifle->Init();
	cARifle->SetShader("Shader3D_Model");
	cPlayer3D->SetWeapon(1, cARifle);
	
	float fCheckHeight = cTerrain->GetHeight(0.0f, -10.0f);
	// Initialise a CStructure3D
	fCheckHeight = cTerrain->GetHeight(2.0f, -2.0f);
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(2.0f, fCheckHeight, -2.0f));
	cStructure3D->SetShader("Shader3D");
	cStructure3D->Init();
	cStructure3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//cStructure3D->SetScale(glm::vec3(0.5f));
	cSolidObjectManager->Add(cStructure3D);
	
	// Initialise a CStructure3D
	for (int i = 0; i < 6; i++)
	{
		float x = Math::RandFloatMinMax(-40.f,40.f);
		float z = Math::RandFloatMinMax(-40.f, 40.f);
		float fCheckHeight1;
		fCheckHeight1 = cTerrain->GetHeight(x, z);
		CSpeedUpbox* cSpeedUpbox = new CSpeedUpbox(glm::vec3(x, fCheckHeight1,z));
		cSpeedUpbox->SetShader("Shader3D");
		cSpeedUpbox->Init();
		cSpeedUpbox->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		cSolidObjectManager->Add(cSpeedUpbox);
	}
	
	for (int i = 0; i < 6; i++)
	{
		float x = Math::RandFloatMinMax(-40.f, 40.f);
		float z = Math::RandFloatMinMax(-40.f, 40.f);
		float fCheckHeight2;
		fCheckHeight2 = cTerrain->GetHeight(x, z);
		CInvincibleBox* cInvincibleBox = new CInvincibleBox(glm::vec3(x, fCheckHeight2, z));
		cInvincibleBox->SetShader("Shader3D");
		cInvincibleBox->Init();
		cInvincibleBox->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		cSolidObjectManager->Add(cInvincibleBox);
	}
	
	for (int i = 0; i < 6; i++)
	{
		float x = Math::RandFloatMinMax(-40.f, 40.f);
		float z = Math::RandFloatMinMax(-40.f, 40.f);
		float fCheckHeight3;
		fCheckHeight3 = cTerrain->GetHeight(x, z);
		CHealthUP* cHealthUP = new CHealthUP(glm::vec3(x, fCheckHeight3, z));
		cHealthUP->SetShader("Shader3D");
		cHealthUP->Init();
		cHealthUP->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		cSolidObjectManager->Add(cHealthUP);
	}
	
	for (int i = 0; i < 8; i++)
	{
		float x = Math::RandFloatMinMax(-50.f, 50.f);
		float z = Math::RandFloatMinMax(-50.f, 50.f);
		float fCheckHeight3;
		fCheckHeight3 = cTerrain->GetHeight(x, z);
		WatchTower* cWatchTower = new WatchTower(glm::vec3(x, fCheckHeight3-2, z));
		cWatchTower->SetShader("Shader3DNoColour");
		cWatchTower->Init();
		cWatchTower->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		cSolidObjectManager->Add(cWatchTower);
	}

	//Load the GUI entities
	//Store the cGUI_Scene3D instance here
	cGUI_Scene3D = CGUI_Scene3D::GetInstance();
	cGUI_Scene3D->Init();

	// Load the non-movable Entities with no collisions
	// Initialise the CEntityManager
	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	cGameManager3D = CGameManager3D::GetInstance();
	cGameManager3D->Init();

	// Initialise a CHut_Concrete
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	//CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	//cHut_Concrete->SetShader("Shader3DNoColour");
	//cHut_Concrete->SetLODStatus(true);
	//cHut_Concrete->Init();
	//cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//
	//// Add the cHut_Concrete to the cSolidObjectManager
	//cSolidObjectManager->Add(cHut_Concrete);

	// Initialise a suv
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	CCar* car = new CCar(glm::vec3(5.0f, fCheckHeight, 0.f));
	car->SetShader("Shader3DNoColour");
	car->SetLODStatus(true);
	car->Init();
	car->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	car->SetType(CEntity3D::CAR);

	// Add the suv to the cSolidObjectManager
	cSolidObjectManager->Add(car);
	
	// Initialise a suv
	//fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	//SUV* suv2 = new SUV(glm::vec3(5.0f, fCheckHeight, 5.0f));
	//suv2->SetShader("Shader3DNoColour");
	//suv2->SetLODStatus(true);
	//suv2->Init();
	//suv2->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//// Add the suv to the cSolidObjectManager
	//cSolidObjectManager->Add(suv2);

	// Initialise a dinosaur
	//fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	//Dinosaur* dino = new Dinosaur(glm::vec3(10.0f, fCheckHeight, 5.0f));
	//dino->SetShader("Shader3DNoColour");
	//dino->SetLODStatus(true);
	//dino->Init();
	//dino->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//// Add the Dinosaur to the cSolidObjectManager
	//cSolidObjectManager->Add(dino);

	// Initialise a dinosaur
	//fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	//Baryonyx* bary = new Baryonyx(glm::vec3(-10.0f, fCheckHeight, -5.0f));
	//bary->SetShader("Shader3DNoColour");
	//bary->SetLODStatus(true);
	//bary->Init();
	//bary->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//// Add the Dinosaur to the cSolidObjectManager
	//cSolidObjectManager->Add(bary);

	// Initialise a dinosaur
	//fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	//Charano* charano = new Charano(glm::vec3(5.0f, fCheckHeight, 10.0f));
	//charano->SetShader("Shader3DNoColour");
	//charano->SetLODStatus(true);
	//charano->Init();
	//charano->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//// Add the Dinosaur to the cSolidObjectManager
	//cSolidObjectManager->Add(charano);

	// Initialise a dinosaur
	//fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	//Edo* edo = new Edo(glm::vec3(30.0f, fCheckHeight, 20.0f));
	//edo->SetShader("Shader3DNoColour");
	//edo->SetLODStatus(true);
	//edo->Init();
	//edo->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//// Add the Dinosaur to the cSolidObjectManager
	//cSolidObjectManager->Add(edo);

	// Initialise the CTreeKabak3D
	CTreeKabak3D* cTreeKabak3D = new CTreeKabak3D(glm::vec3(0.0f, 0.0f, 0.0f));
	cTreeKabak3D->SetInstancingMode(true);
	if (cTreeKabak3D->IsInstancedRendering() == true)
	{
		cTreeKabak3D->SetScale(glm::vec3(1.0f));
		cTreeKabak3D->SetNumOfInstance(100);
		cTreeKabak3D->SetSpreadDistance(100.0f);

		cTreeKabak3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	if (cTreeKabak3D->Init() == true)
	{
		cEntityManager->Add(cTreeKabak3D);
	}
	else
	{
		delete cTreeKabak3D;
	}

	// Initialise a CSpinTower
	//CSpinTower::Create();
	//Wheel::Create();
	//LVTower::Create();
	//Planet::Create();
	//Decor::Create();

	Enemy();

	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	//Store the position for rollback if needed
	cPlayer3D->StorePositionForRollback();

	//if ()

	if (cPlayer3D->GetSMovement() != CPlayer3D::PLAYER_SMOVEMENT::CAR)
	{
		PlayerControlsUpdate(dElapsedTime);
	}

	//Get keyboard and mouse updates
	if (!cPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);

		// Get mouse updates
		cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}
	else
	{
		// Get mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	//Detach camera
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
		{
			cPlayer3D->AttachCamera();
			cout << "not attached" << endl;
		}
		else
		{
			cPlayer3D->AttachCamera(cCamera);
			cout << "attached" << endl;
		}

		//Reset key so that it will not repeat until key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}

	//Update the solid objects
	cSolidObjectManager->Update(dElapsedTime);

	//Update the projectiles
	cProjectileManager->Update(dElapsedTime);

	//Check for collision
	cSolidObjectManager->CheckForCollision();

	// Update the entities
	cEntityManager->Update(dElapsedTime);

	//Call cGUI_Scene3D's update method
	cGUI_Scene3D->Update(dElapsedTime);

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	// Part 1: Render for the minimap by binding to framebuffer and render to color texture
	//         But the camera is move to top-view of the scene

	// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;

	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	cCamera->fYaw += 180.0f;
	cCamera->fPitch = -90.0f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	cCamera->vec3Position = glm::vec3(storePlayerPosition.x, 10.0f, storePlayerPosition.z);
	// Recalculate all the camera vectors. 
	// We disable pitch constrains for this specific case as we want the camera to look straight down
	cCamera->ProcessMouseMovement(0, 0, false);

	// Generate the view and projection
	glm::mat4 playerView = cCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(glm::radians(45.0f),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);

	// Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)

	// Activate the minimap system
	CMinimap::GetInstance()->Activate();
	// Setup the rendering environment
	CMinimap::GetInstance()->PreRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render the Terrain
	cTerrain->SetView(playerView);
	cTerrain->SetProjection(playerProjection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities
	cEntityManager->SetView(playerView);
	cEntityManager->SetProjection(playerProjection);
	cEntityManager->Render();

	// Render the entities for the minimap
	cSolidObjectManager->SetView(playerView);
	cSolidObjectManager->SetProjection(playerProjection);
	cSolidObjectManager->Render();

	// Deactivate the cMinimap so that we can render as per normal
	CMinimap::GetInstance()->Deactivate();

							 // Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
												(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
												0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render SkyBox
	cSkyBox->SetView(view);
	cSkyBox->SetProjection(projection);
	cSkyBox->PreRender();
	cSkyBox->Render();
	cSkyBox->PostRender();

	//Render Terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities
	cEntityManager->SetView(view);
	cEntityManager->SetProjection(projection);
	cEntityManager->Render();

	//Render Solid Object Manager
	cSolidObjectManager->SetView(view);
	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->Render();

	//Render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();

	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cGUI_Scene3D->SetProjection(projection);
	// Call the cGUI_Scene3D's PreRender()
	cGUI_Scene3D->PreRender();
	// Call the cGUI_Scene3D's Render()
	cGUI_Scene3D->Render();
	// Call the cGUI_Scene3D's PostRender()
	cGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}

void CScene3D::PlayerControlsUpdate(const double dElapsedTime)
{
	
	//if (!cPlayer3D->IsCameraAttached())
	//{
	//	cPlayer3D->AttachCamera(cCamera);
	//}
	
	//Update Slide
	if (cPlayer3D->GetSlideTime() > 0.0f && cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::SLIDE)
	{
		cPlayer3D->SlideMovement((float)dElapsedTime);
	}

	//Movement
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W) && CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_CAPS_LOCK))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::WALK, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S) && CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_CAPS_LOCK))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::WALKBACKWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);

	//Jump
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_SPACE))
	{
		if (cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::SLIDE)
		{
			cPlayer3D->SetSlideTime(0.0f);
			cPlayer3D->SetSMovement(CPlayer3D::PLAYER_SMOVEMENT::STAND);
		}
		cPlayer3D->SetToJump();
	}

	//Sprint
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT) && cPlayer3D->GetSlowed()==false && cPlayer3D->GetIsSpeed() == false)
	{
		cPlayer3D->SetMovementSpeed(15);
		CCameraEffectsManager::GetInstance()->Get("SweatScreen")->SetStatus(true);
		cSoundController->PlaySoundByID(6);
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT) && cPlayer3D->GetIsSpeed() == true)
	{
		cPlayer3D->SetMovementSpeed(35);
		CCameraEffectsManager::GetInstance()->Get("SweatScreen")->SetStatus(true);
		cSoundController->PlaySoundByID(6);
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_LEFT_SHIFT) && cPlayer3D->GetIsSpeed() == false)
	{
		cPlayer3D->SetMovementSpeed(CPlayer3D::GetInstance()->PLAYER_SPEED);
		CCameraEffectsManager::GetInstance()->Get("SweatScreen")->SetStatus(false);
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_LEFT_SHIFT) && cPlayer3D->GetIsSpeed() == true)
	{
		cPlayer3D->SetMovementSpeed(CPlayer3D::GetInstance()->PLAYER_SPEED *3);
		CCameraEffectsManager::GetInstance()->Get("SweatScreen")->SetStatus(false);
	}

	//Slide
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_G) && cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::STAND)
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("DirtScreen")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("DirtScreen")->SetStatus(!bStatus);

		cPlayer3D->SlideMovement((float)dElapsedTime);
		cSoundController->PlaySoundByID(7);
	}


	//SMovements
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_C))
	{
		if (cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::STAND)
		{
			bool bStatus = CCameraEffectsManager::GetInstance()->Get("DirtScreen")->GetStatus();
			CCameraEffectsManager::GetInstance()->Get("DirtScreen")->SetStatus(!bStatus);

			cPlayer3D->SetMovementSpeed(DEFAULT_STAND * 0.5f);
			cPlayer3D->SetSMovement(CPlayer3D::PLAYER_SMOVEMENT::CROUCH);
		}
		else if (cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::CROUCH || cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::SLIDE)
		{
			if (cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::SLIDE)
			{
				//bool bStatus = CCameraEffectsManager::GetInstance()->Get("DirtScreen")->GetStatus();
				//CCameraEffectsManager::GetInstance()->Get("DirtScreen")->SetStatus(!bStatus);
				cPlayer3D->SetSlideTime(0.0f);
			}
			bool bStatus = CCameraEffectsManager::GetInstance()->Get("DirtScreen")->GetStatus();
			CCameraEffectsManager::GetInstance()->Get("DirtScreen")->SetStatus(!bStatus);

			cPlayer3D->SetMovementSpeed(DEFAULT_STAND);
			cPlayer3D->SetSMovement(CPlayer3D::PLAYER_SMOVEMENT::STAND);
		}
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_V))
	{
		if (cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::CROUCH)
		{
			cPlayer3D->SetMovementSpeed(DEFAULT_STAND * 0.25f);
			cPlayer3D->SetSMovement(CPlayer3D::PLAYER_SMOVEMENT::PRONE);
		}
		else if (cPlayer3D->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::PRONE)
		{
			cPlayer3D->SetMovementSpeed(DEFAULT_STAND * 0.5f);
			cPlayer3D->SetSMovement(CPlayer3D::PLAYER_SMOVEMENT::CROUCH);
		}
	}

	//Switch weapon
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
	{
		cPlayer3D->SetCurrentWeapon(0);
	}

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
	{
		cPlayer3D->SetCurrentWeapon(1);
	}

	//if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_3))
	//{
	//	cPlayer3D->SetCurrentWeapon(2);
	//}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_8))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CameraShake")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(!bStatus);

		//Reset the key so that it will not repeat until the key is pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_8);
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CrossHair")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(!bStatus);

		//Reset the key so that it will not repeat until the key is pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_9);
	}

	//Reload
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R))
	{
		cPlayer3D->GetWeapon()->Reload();
		cSoundController->PlaySoundByID(5);
	}

	//Get mouse button updates
	switch (cPlayer3D->GetWeapon()->GetWeaponType())
	{
	case CWeaponInfo::WEAPON_TYPE::CARD:
		if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB))
		{
			if (cPlayer3D->GetWeapon()->GetMaxChargeTime() > cPlayer3D->GetWeapon()->GetChargeTime())
			{
				cPlayer3D->GetWeapon()->SetChargeTime(cPlayer3D->GetWeapon()->GetChargeTime() + dElapsedTime);
			}
			
		}
		else if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::LMB))
		{
			cPlayer3D->DischargeWeapon(); //Release based on charge
			cPlayer3D->GetWeapon()->SetChargeTime(0.0f);
		}
		break;
	case CWeaponInfo::WEAPON_TYPE::ASSAULT_RIFLE:
		if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB))
		{
			cPlayer3D->DischargeWeapon();
		}
		break;
	}

	if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::RMB))
	{
		//Switch on scope mode and zoom in
		cCamera->fZoom = 1.0f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
	}
	else if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::RMB))
	{
		//Switch on scope mode and zoom in
		cCamera->fZoom = 45.0f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
	}
}

void CScene3D::Enemy(void)
{
	// Initialise the cEnemy3D
	float fCheckHeight = cTerrain->GetHeight(0.0f, -10.0f);
	CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(0.0f, fCheckHeight, -10.0f));
	cEnemy3D->SetShader("Shader3D");
	cEnemy3D->Init();
	cEnemy3D->SetRotation(-180.f, glm::vec3(0.0f, 1.0f, 0.0f));
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//cEnemy3D->SetScale(glm::vec3(0.5f));

	// Assign a cPistol to the cEnemy3D
	CPistol* cEnemyPistol = new CPistol();
	// Set the position, rotation and scale of this weapon
	//cEnemyPistol->SetPosition(glm::vec3(0.05f, -0.075f, 0.5f));
	//cEnemyPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	// Initialise the instance
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyPistol);
	cEnemy3D->SetMaxHP(200);

	cSolidObjectManager->Add(cEnemy3D);
	cSolidObjectManager->SetEnemyCount(cSolidObjectManager->GetEnemyCount() + 1);

	// Initialise the Spider
	fCheckHeight = cTerrain->GetHeight(0.0f, -10.0f);
	Spider* spider = new Spider(glm::vec3(0.0f, fCheckHeight, -20.0f));
	spider->SetShader("Shader3DNoColour");
	spider->Init();
	spider->SetRotation(-180.f, glm::vec3(0.0f, 1.0f, 0.0f));
	spider->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	////cEnemy3D->SetScale(glm::vec3(0.5f));
	//
	// Assign a cPistol to the cEnemy3D
	CPistol* spiderPistol = new CPistol();
	// Set the position, rotation and scale of this weapon
	//cEnemyPistol->SetPosition(glm::vec3(0.05f, -0.075f, 0.5f));
	//cEnemyPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	spiderPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	// Initialise the instance
	spiderPistol->Init();
	spiderPistol->SetShader("Shader3D_Model");
	spider->SetWeapon(0, spiderPistol);
	spider->SetMaxHP(60);
	cSolidObjectManager->Add(spider);
	cSolidObjectManager->SetEnemyCount(cSolidObjectManager->GetEnemyCount() + 1);

	// Initialise the Soldier
	fCheckHeight = cTerrain->GetHeight(0.0f, -10.0f);
	Soldier* soldier = new Soldier(glm::vec3(0.0f, fCheckHeight, -20.0f));
	soldier->SetShader("Shader3DNoColour");
	soldier->Init();
	soldier->SetRotation(-180.f, glm::vec3(0.0f, 1.0f, 0.0f));
	soldier->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	////cEnemy3D->SetScale(glm::vec3(0.5f));
	//
	// Assign a cPistol to the cEnemy3D
	CPistol* soldierPistol = new CPistol();
	// Set the position, rotation and scale of this weapon
	//cEnemyPistol->SetPosition(glm::vec3(0.05f, -0.075f, 0.5f));
	//cEnemyPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	soldierPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	// Initialise the instance
	soldierPistol->Init();
	soldierPistol->SetShader("Shader3D_Model");
	soldier->SetWeapon(0, soldierPistol);
	soldier->SetMaxHP(40);
	cSolidObjectManager->Add(soldier);
	cSolidObjectManager->SetEnemyCount(cSolidObjectManager->GetEnemyCount() + 1);
}
