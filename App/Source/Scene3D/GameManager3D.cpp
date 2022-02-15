#include "GameManager3D.h"

/**
@brief Constructor
*/
CGameManager3D::CGameManager3D(void)
	: bPlayerWon(false)
	, bPlayerLost(false)
	, bLevelCompleted(false)
	, bLevelToReplay(false)
	, bGameToRestart(false) {
}

/**
@brief Destructor
*/
CGameManager3D::~CGameManager3D(void)
{
}

// Initialise this class instance
void CGameManager3D::Init(void)
{
	bPlayerWon = false;
	bPlayerLost = false;
	bLevelCompleted = false;
	bLevelToReplay = false;
	bGameToRestart = false;
}

