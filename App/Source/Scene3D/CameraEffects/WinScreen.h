#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CWinScreen : public CCameraEffects
{
public:
	// Constructor
	CWinScreen(void);
	// Destructor
	virtual ~CWinScreen(void);

	// Initialise this class instance
	virtual bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
