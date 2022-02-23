/**
CGameManager: This class manages the gameplay and determines 
			  if it should go to next level or end.
By: Toh Da Jun
Date: May 2021
*/
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <map>
#include <string>

class CGameManager3D : public CSingletonTemplate<CGameManager3D>
{
	friend CSingletonTemplate<CGameManager3D>;
public:
	// Initialise this class instance
	void Init(void);

	// Boolean flag to indicate if the player won the game
	bool bPlayerWon;
	// Boolean flag to indicate if the player lost the game
	bool bPlayerLost;
	// Boolean flag to indicate if the level is completed
	bool bLevelCompleted;

	// Boolean flag to indicate if the level is be replayed
	bool bLevelToReplay;
	// Boolean flag to indicate if the game is be restarted
	bool bGameToRestart;

	//Current Level in scene3D
	int iCurrentLevel;
	int iAmtOfEnemies;

	//Round Timer;
	float fRoundTimer;
	float fMaxRoundTimer;

	void UpdateRoundTimer(const double dElapsedTime);
protected:
	// Constructor
	CGameManager3D(void);

	

	// Destructor
	virtual ~CGameManager3D(void);
};
