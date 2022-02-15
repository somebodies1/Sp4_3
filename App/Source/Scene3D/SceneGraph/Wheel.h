/**
 CSpinTower
 By: Toh Da Jun
 Date: Sept 2021
 */
#pragma once

// Include CSceneNode
#include "SceneNode.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

// A tower which spins
class Wheel
{
public:
	// Create the Spin Tower using this static method
	static bool Create(void);

	// PrintSelf
	void PrintSelf(void);

protected:
	// Constructor
	Wheel(void);

	// Destructor
	virtual ~Wheel(void);
};
