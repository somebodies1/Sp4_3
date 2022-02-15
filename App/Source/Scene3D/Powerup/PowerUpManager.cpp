#include "PowerUpManager.h"

PowerUpManager::PowerUpManager()
{
}

PowerUpManager::~PowerUpManager()
{
}

void PowerUpManager::UpdatePowerUpEffect(double dt)
{

}

float PowerUpManager::GetSpawnTimer()
{
	return 0.0f;
}

void PowerUpManager::SetSpawnTimer(float newTime)
{
}

void PowerUpManager::InsertPowerUpInList(powerup* pUp)
{
}

void PowerUpManager::RemovePowerUpInList(powerup* pUp)
{
}

std::vector<powerup*> PowerUpManager::GetActivePowerUpList()
{
	return std::vector<powerup*>();
}
