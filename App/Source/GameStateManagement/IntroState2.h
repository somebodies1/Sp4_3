#pragma once

/**
 CIntroState2
 @brief This class is derived from CGameStateBase. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"

#include "Primitives/Mesh.h"
#include "../Scene2D/BackgroundEntity.h"
#include <string>

class CIntroState2 : public CGameStateBase
{
public:
	// Constructor
	CIntroState2(void);
	// Destructor
	~CIntroState2(void);

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
