#pragma once
#include "DesignPatterns/SingletonTemplate.h"
#include "powerup.h"
#include "../Entities/Player3D.h"
#include <vector>

//Created by Ridwan
class PowerUpManager : public CSingletonTemplate<PowerUpManager>
{
	friend CSingletonTemplate<PowerUpManager>;
private:
	//Power Up
	std::vector<powerup*> activepowerList;

	//spawn timers
	float spawntimer;
	

	CPlayer3D* player;
public:
	PowerUpManager();
	~PowerUpManager();

	void UpdatePowerUpEffect(double dt);

	float GetSpawnTimer();
	void SetSpawnTimer(float newTime);


	void InsertPowerUpInList(powerup* pUp);
	void RemovePowerUpInList(powerup* pUp);
	std::vector<powerup*> GetActivePowerUpList();
};

