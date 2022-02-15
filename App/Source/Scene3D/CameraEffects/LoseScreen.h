#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CLoseScreen : public CCameraEffects
{
public:
	// Constructor
	CLoseScreen(void);
	// Destructor
	virtual ~CLoseScreen(void);

	// Initialise this class instance
	virtual bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
