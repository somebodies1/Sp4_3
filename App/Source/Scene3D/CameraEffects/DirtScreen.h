#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CDirtScreen : public CCameraEffects
{
public:
	// Constructor
	CDirtScreen(void);
	// Destructor
	virtual ~CDirtScreen(void);

	// Initialise this class instance
	virtual bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
