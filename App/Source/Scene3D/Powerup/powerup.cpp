#include "powerup.h"

powerup::powerup()
{
	powertype = NUM_POWERUPTYPE;
	timeLeft = 5.f;
	dead = false;
}

powerup::powerup(POWERUPTYPE power1, float timeL)
{
	powertype = power1;
	timeLeft = timeL;
	dead = false;
}

powerup::~powerup()
{
}



powerup::POWERUPTYPE powerup::getpowertype()
{
	return powertype;
}

void powerup::Speed( double dt)
{
	decreaseTime(dt);
	if (timeLeft <= 0.f)
	{
		//owner->SetMovementSpeed(owner->GetMovementSpeed() * 2.0f);
	}
	else
	{
		//owner->SetMovementSpeed(owner->GetMovementSpeed() * 0.5f);
	}
	return;
}

powerup::POWERUPTYPE powerup::Update(double dt)
{
	switch (powertype)
	{
	case SPEED:
		decreaseTime(dt);
		break;
	case INVINCIBLE:
		decreaseTime(dt);
		break;
	case SLOWED:
		decreaseTime(dt);
		break;
	case HEALTHUP:
		decreaseTime(dt);
		break;
	}
	return powertype;
}

void powerup::setpowertype(POWERUPTYPE power)
{
	this->powertype = power;
}

void powerup::setTimeLeft(float timeL)
{
	timeLeft = timeL;
}

float powerup::getTimeLeft()
{
	return timeLeft;
}

void powerup::decreaseTime(double dt)
{
	timeLeft -= dt;
}

void powerup::setDead(bool dead)
{
	this->dead = dead;
}

bool powerup::getDead()
{
	return dead;
}
