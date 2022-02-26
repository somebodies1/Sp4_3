#pragma once

/**
 CIntroState
 @brief This class is derived from CGameStateBase. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"

#include "Primitives/Mesh.h"
#include "../Scene2D/BackgroundEntity.h"
#include <string>

#include "../App/Source/SoundController/SoundController.h"
#include "../Library/Source/System/filesystem.h"

class CIntroState : public CGameStateBase
{
public:
	CSoundController* cSoundController;

	// Constructor
	CIntroState(void);
	// Destructor
	~CIntroState(void);

	// Init this class instance
	virtual bool Init(void);
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

protected:
	CBackgroundEntity* background;
};
