#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CCard :	public CWeaponInfo
{
public:
	// Default Constructor
	CCard(void);
	// Destructor
	virtual ~CCard(void);

	// Initialise this instance to default values
	bool Init(void);
};

