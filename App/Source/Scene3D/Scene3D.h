/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CKeyboardController
#include "Inputs\KeyboardController.h"
// Include CMouseController
#include "Inputs\MouseController.h"

// Include CFPSCounter
#include "TimeControl\FPSCounter.h"

// Include CSoundController
#include "../SoundController/SoundController.h"

// Include CCamera
#include "Camera.h"

//Include CSkyBox
#include "SkyBox/SkyBox.h"

//Include CTerrain
#include "Terrain/Terrain.h"

//Include CSolidObjectManager
#include "Entities/SolidObjectManager.h"

//Include CPlayer3D
#include "Entities/Player3D.h"

//Incldue CProjectileManager
#include "WeaponInfo/ProjectileManager.h"

//Include Weapons
#include "WeaponInfo/Pistol.h"
#include "WeaponInfo/Card.h"
#include "WeaponInfo/AssaultRifle.h"

//Include CStructure3D
#include "Entities/Structure3D.h"
#include "Entities/SpeedUpbox.h"
#include "Entities/InvincibleBox.h"
#include "Entities/HealthUP.h"
#include "Entities/WatchTower.h"

//Include Enemies
#include "Entities/Enemy3D.h"
#include "Entities/Spider.h"
#include "Entities/Soldier.h"

//Include GUI_Scene3D
#include "GUI_Scene3D.h"

//Include CCameraShake
#include "CameraEffects/CameraShake.h"

// Include CEntityManager
#include "Entities\EntityManager.h"
#include "GameManager3D.h"
#include "../GameStateManagement/GameStateManager.h"

class CSettings;

class CScene3D : public CSingletonTemplate<CScene3D>
{
	friend CSingletonTemplate<CScene3D>;
public:
	// Init
	bool Init(void);

	// Update
	bool Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	void PlayerControlsUpdate(const double dElapsedTime);

	void Enemy(void);

protected:
	// The handler to the CSettings
	CSettings* cSettings;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	// Mouse Controller singleton instance
	CMouseController* cMouseController;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// FPS Control
	CFPSCounter* cFPSCounter;

	// Handler to the CSoundController
	CSoundController* cSoundController;

	// Handler to the camera class
	CCamera* cCamera;

	//Handler to CSkyBox
	CSkyBox* cSkyBox;

	//Handler to CTerrain
	CTerrain* cTerrain;

	//Handler to CSolidObjectManager class
	CSolidObjectManager* cSolidObjectManager;

	//Handler to Player3D class
	CPlayer3D* cPlayer3D;

	//Handler to CProjectileManager instance
	CProjectileManager* cProjectileManager;

	//Handler to CGUI_Scene3D
	CGUI_Scene3D* cGUI_Scene3D;

	// Handler to the EntityManager class
	CEntityManager* cEntityManager;

	//Handler to gameManager class
	CGameManager3D* cGameManager3D;

	CGameStateManager* cGameStateManager;

	// Constructor
	CScene3D(void);
	// Destructor
	virtual ~CScene3D(void);
};

