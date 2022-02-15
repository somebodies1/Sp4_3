/**
 CSweatScreen
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects lxzh long xie pand xie
#include "CameraEffects.h"

class CSweatScreen : public CCameraEffects
{
public:
	// Constructor
	CSweatScreen(void);
	// Destructor
	virtual ~CSweatScreen(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
