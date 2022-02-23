#include "GameManager3D.h"

/**
@brief Constructor
*/
CGameManager3D::CGameManager3D(void)
	: bPlayerWon(false)
	, bPlayerLost(false)
	, bLevelCompleted(false)
	, bLevelToReplay(false)
	, bGameToRestart(false)
	, iCurrentLevel(0)
	, fRoundTimer(5.0f)
	, fMaxRoundTimer(5.0f)
	, iAmtOfEnemies(0){
}

void CGameManager3D::UpdateRoundTimer(const double dElapsedTime)
{
	fRoundTimer -= dElapsedTime;
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
	iCurrentLevel = 0;

	fRoundTimer = 5.0f;
	fMaxRoundTimer = 5.0f;
	iAmtOfEnemies = 0;
}

