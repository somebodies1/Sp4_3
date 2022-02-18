#pragma once
//#include "Primitives\Entity3D.h"
#include<vector>
#include<iostream>
#include <string>

//Created by Zhong Heng
class powerup
{
public:
	enum POWERUPTYPE
	{
		SPEED = 0,
		INVINCIBLE,
		FIRERATE,
		TIMESTOP,
		SLOWED,
		HEALTHUP,
		NUM_POWERUPTYPE
	};
	
	powerup();
	powerup(POWERUPTYPE power1, float timeL);
	~powerup();

	powerup::POWERUPTYPE Update(double dt);
	void setpowertype(POWERUPTYPE power);
	POWERUPTYPE getpowertype();

	void Speed(double dt);

	void setTimeLeft(float timeL);
	float getTimeLeft();
	void decreaseTime(double dt);

	void setDead(bool dead);
	bool getDead();
	
protected:
	float timeLeft;
	POWERUPTYPE powertype;
	bool dead;
};