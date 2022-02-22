/**
 CSolidObjectManager
 By: Toh Da Jun
 Date: Apr 2020
 */

#include "SolidObjectManager.h"

// Include CCollisionManager
#include "Primitives/CollisionManager.h"

// Include CCameraEffectsManager
//#include "../CameraEffects/CameraEffectsManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CSolidObjectManager::CSolidObjectManager(void)
	: model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, cProjectileManager(NULL)
	, cInventoryManager(NULL)
	, cInventoryItem(NULL)
	, cGameManager3D(NULL)
{
}

/**
 @brief Destructor
 */
CSolidObjectManager::~CSolidObjectManager(void)
{
	// We won't delete this since it was created elsewhere
	cProjectileManager = NULL;

	// We won't delete this since it was created elsewhere
	cInventoryManager = NULL;

	cGameManager3D = NULL;

	// Remove all CSolidObject
	std::list<CSolidObject*>::iterator it = lSolidObject.begin(), end = lSolidObject.end();
	while (it != end)
	{
		// Delete if done
		delete *it;
		it = lSolidObject.erase(it);
	}
}

/**
 @brief Initialise this class instance
 @return A bool variable
 */
bool CSolidObjectManager::Init(void)
{
	lSolidObject.clear();

	cProjectileManager = CProjectileManager::GetInstance();
	cPlayer3D = CPlayer3D::GetInstance();

	// Get the handler to the CInventoryManager instance
	cInventoryManager = CInventoryManager::GetInstance();
	enemyCount = 0;

	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\debuff.ogg"), 1, true, false, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\itempickup.ogg"), 10, true, false, true);
	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Add a CSolidObject* to this class instance
 @param cSolidObject The CSolidObject* variable to be added to this class instance
 */
void CSolidObjectManager::Add(CSolidObject* cSolidObject)
{
	lSolidObject.push_back(cSolidObject);
}

/**
 @brief Remove a CSolidObject* from this class instance
 @param cSolidObject The CSolidObject* variable to be erased from this class instance
 @return A bool variable
 */
bool CSolidObjectManager::Erase(CSolidObject* cSolidObject)
{
	// Find the entity's iterator
	std::list<CSolidObject*>::iterator findIter = std::find(lSolidObject.begin(), lSolidObject.end(), cSolidObject);

	// Delete the entity if found
	if (findIter != lSolidObject.end())
	{
		// Delete the CSolidObject
		//delete *findIter;
		// Go to the next iteration after erasing from the list
		findIter = lSolidObject.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

/**
 @brief Collision Check for a CSolidObject*
 @param cSolidObject The CSolidObject* variable to be checked
 @return A bool variable
 */
bool CSolidObjectManager::CollisionCheck(CSolidObject* cSolidObject)
{
	std::list<CSolidObject*>::iterator it, end;
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// Check for collisions between the 2 entities
		if (CCollisionManager::BoxBoxCollision( cSolidObject->GetPosition() + cSolidObject->boxMin,
												cSolidObject->GetPosition() + cSolidObject->boxMax,
												(*it)->GetPosition() + (*it)->boxMin,
												(*it)->GetPosition() + (*it)->boxMax) == true)
		{
			// Rollback the cSolidObject's position
			cSolidObject->RollbackPosition();
			// Rollback the NPC's position
			(*it)->RollbackPosition();
			if ((*it)->GetType() == CSolidObject::TYPE::NPC)
				cout << "** Collision between this Entity and a NPC ***" << endl;
			else if ((*it)->GetType() == CSolidObject::TYPE::OTHERS)
				cout << "** Collision between this Entity and an OTHERS ***" << endl;
			else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE)
				cout << "** Collision between this Entity and a STRUCTURE ***" << endl;
			else if ((*it)->GetType() == CSolidObject::TYPE::PROJECTILE)
				cout << "** Collision between this Entity and a PROJECTILE ***" << endl;
			// Quit this loop since a collision has been found
			break;
		}
	}

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CSolidObjectManager::Update(const double dElapsedTime)
{
	std::list<CSolidObject*>::iterator it, end;

	// Update all CSolidObject
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		(*it)->Update(dElapsedTime);
	}

	return true;
}

/**
 @brief Collision Check for all entities and projectiles
 @return A bool variable
 */
bool CSolidObjectManager::CheckForCollision(void)
{
	bool bResult = false;

	std::list<CSolidObject*>::iterator it, end;
	std::list<CSolidObject*>::iterator it_other;

	// Check for collisions among entities
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// If the entity is not active, then skip it
		if ((*it)->GetStatus() == false)
			continue;

		for (it_other = lSolidObject.begin(); it_other != end; ++it_other)
		{
			// If the entity is not active, then skip it
			if ((*it_other)->GetStatus() == false)
				continue;

			// If the 2 entities to check are the same, then skip this iteration
			if (it_other == it)
				continue;

			// Check for collisions between the 2 entities
			if (CCollisionManager::BoxBoxCollision((*it)->GetPosition() + (*it)->boxMin,
				(*it)->GetPosition() + (*it)->boxMax,
				(*it_other)->GetPosition() + (*it_other)->boxMin,
				(*it_other)->GetPosition() + (*it_other)->boxMax) == true)
			{
				// Check if a movable entity collides with another movable entity
				if (
					((*it)->GetType() == CSolidObject::TYPE::PLAYER)
					&&
					(((*it_other)->GetType() == CSolidObject::TYPE::NPC) &&
						((*it_other)->GetType() == CSolidObject::TYPE::OTHERS))
					)
				{
					(*it)->RollbackPosition();
					(*it_other)->RollbackPosition();
					//bResult = true;
					cout << "** Collision between Player and an Entity ***" << endl;
					break;
				}
				else if (
						(((*it)->GetType() == CSolidObject::TYPE::NPC) &&
						((*it)->GetType() == CSolidObject::TYPE::OTHERS))
						&&
						(((*it_other)->GetType() == CSolidObject::TYPE::NPC) &&
						((*it_other)->GetType() == CSolidObject::TYPE::OTHERS))
					)
				{
					(*it)->RollbackPosition();
					(*it_other)->RollbackPosition();
					cout << "** Collision between 2 Entities ***" << endl;
					break;
				}
				// Check if a movable entity collides with a non-movable entity
				if (
						(((*it)->GetType() == CSolidObject::TYPE::PLAYER) &&
							((*it)->GetType() == CSolidObject::TYPE::OTHERS))
						&&
						((*it_other)->GetType() == CSolidObject::TYPE::STRUCTURE)
					)
				{
					(*it)->RollbackPosition();
					if (((*it)->GetType() == CSolidObject::TYPE::PLAYER))
						//bResult = true;
					cout << "** Collision between Entity and Structure ***" << endl;
					break;
				}
				//statusEffects
				if (
					(((*it)->GetType() == CSolidObject::TYPE::PLAYER) &&
					((*it_other)->GetType() == CSolidObject::TYPE::SPEED_UP)
					))
				{
					(*it)->RollbackPosition();
					(*it)->AddPowerup((*it),powerup::SPEED, 5.f);
					//if (((*it)->GetType() == CSolidObject::TYPE::PLAYER))
						//bResult = true;
					std::cout << (*it)->GetMovementSpeed() << std::endl;
					(*it_other)->SetStatus(false);
						cout << "** Collision between Entity and powerUP ***" << endl;
						cSoundController->PlaySoundByID(10);
					break;
				}
				if (
					(((*it)->GetType() == CSolidObject::TYPE::PLAYER) &&
						((*it_other)->GetType() == CSolidObject::TYPE::INVINCIBLE_UP)
						))
				{
					(*it)->RollbackPosition();
					(*it)->AddPowerup((*it), powerup::INVINCIBLE, 9.f);
					//if (((*it)->GetType() == CSolidObject::TYPE::PLAYER))
						//bResult = true;
					std::cout << (*it)->GetMovementSpeed() << std::endl;
					(*it_other)->SetStatus(false);
					
					//cInventoryItem = cInventoryManager->GetItem("Health");
					//cInventoryItem->Add(30);
					cout << "** Collision between Entity and powerUP ***" << endl;
					cSoundController->PlaySoundByID(10);
					break;
				}
				if (
					(((*it)->GetType() == CSolidObject::TYPE::PLAYER) &&
						((*it_other)->GetType() == CSolidObject::TYPE::HEALTH_UP)
						))
				{
					(*it)->RollbackPosition();
					(*it)->AddPowerup((*it), powerup::HEALTHUP, 2.f);
					//if (((*it)->GetType() == CSolidObject::TYPE::PLAYER))
						//bResult = true;
					
					(*it_other)->SetStatus(false);

					cInventoryItem = cInventoryManager->GetItem("Health");
					cInventoryItem->Add(30);
					cout << "** Collision between Entity and powerUP ***" << endl;
					cSoundController->PlaySoundByID(10);
					break;
				}
				if (
					((*it)->GetType() == CSolidObject::TYPE::PLAYER)
					&&((*it_other)->GetType() == CSolidObject::TYPE::CAR))
					
				{
					(*it)->RollbackPosition();
					if (((*it)->GetType() == CSolidObject::TYPE::PLAYER))
						//bResult = true;
						cout << "** Collision between Entity and Structure ***" << endl;
					break;
				}
			}
		}
	}

	// Check for collisions between entities and projectiles
	end = lSolidObject.end();

	cInventoryItem = cInventoryManager->GetItem("Health");
	
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// If the entity is not active, then skip it
		if ((*it)->GetStatus() == false)
			continue;

		for (unsigned int i = 0; i < cProjectileManager->vProjectile.size(); i++)
		{
			// If the entity is not active, then skip it
			if ((cProjectileManager->vProjectile[i])->GetStatus() == false)
				continue;

			// Use ray tracing to check for collisions between the 2 entities
			if (CCollisionManager::RayBoxCollision((*it)->GetPosition() + (*it)->boxMin,
				(*it)->GetPosition() + (*it)->boxMax,
				(cProjectileManager->vProjectile[i])->GetPreviousPosition(),
				(cProjectileManager->vProjectile[i])->GetPosition()))
			{
				if ((*it)->GetType() == CSolidObject::TYPE::PLAYER)
				{
					// If this projectile is fired by the player, then skip it
					if ((cProjectileManager->vProjectile[i])->GetSource() == (*it))
						continue;
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::CARD_PROJECTILE && (*it)->GetInvis()==false)
					{
						cInventoryItem->Remove(20);
					}
					else if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::PROJECTILE && (*it)->GetInvis() == false)
					{
						cInventoryItem->Remove(10);
					}
					

					if (cInventoryItem->GetCount() <= 0)
					{
						// Player loses the game
						CGameManager3D::GetInstance()->bPlayerLost = true;
					}
					(*it)->AddPowerup((*it), powerup::SLOWED,1.f);
					cout << "** RayBoxCollision between Player and Projectile ***" << endl;
					bResult = true;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::NPC || (*it)->GetType() == CSolidObject::TYPE::SPIDER)
				{
					// If this projectile is fired by the NPC, then skip it
					if ((cProjectileManager->vProjectile[i])->GetSource() == (*it))
						continue;
					//(*it)->SetStatus(false);
					if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::CARD_PROJECTILE)
					{
						if ((*it)->DamageCheck(20))
						{
							SetEnemyCount(GetEnemyCount() - 1);
							cPlayer3D->SetScore(cPlayer3D->GetScore() + 3);
						}
							
					}
					else if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::PROJECTILE)
					{
						if ((*it)->DamageCheck(10))
						{
							SetEnemyCount(GetEnemyCount() - 1);
							cPlayer3D->SetScore(cPlayer3D->GetScore() + 1);
						}
					}
					
					if (GetEnemyCount() <= 0)
						CGameManager3D::GetInstance()->bPlayerWon = true;
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** RayBoxCollision between NPC and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE)
				{
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** RayBoxCollision between Structure and Projectile ***" << endl;
					break;
				}
			}

			// Check for collisions between the 2 entities
			if (CCollisionManager::BoxBoxCollision((*it)->GetPosition() + (*it)->boxMin,
				(*it)->GetPosition() + (*it)->boxMax,
				(cProjectileManager->vProjectile[i])->GetPosition() + (cProjectileManager->vProjectile[i])->boxMin,
				(cProjectileManager->vProjectile[i])->GetPosition() + (cProjectileManager->vProjectile[i])->boxMax) == true)
			{
				if ((*it)->GetType() == CSolidObject::TYPE::PLAYER)
				{
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::CARD_PROJECTILE)
					{
						cInventoryItem->Remove(20);
					}
					else if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::PROJECTILE)
					{
						cInventoryItem->Remove(10);
					}

					if (cInventoryItem->GetCount() <= 0)
					{
						// Player loses the game
						CGameManager3D::GetInstance()->bPlayerLost = true;
					}
					cout << "** BoxBoxCollision between Player and Projectile ***" << endl;
					bResult = true;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::NPC)
				{
					//(*it)->SetStatus(false);
					if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::CARD_PROJECTILE)
					{
						if ((*it)->DamageCheck(20))
							SetEnemyCount(GetEnemyCount() - 1);
					}
					else if (cProjectileManager->vProjectile[i]->GetType() == CEntity3D::PROJECTILE)
					{
						if ((*it)->DamageCheck(10))
							SetEnemyCount(GetEnemyCount() - 1);
					}

					if (GetEnemyCount() <= 0)
						CGameManager3D::GetInstance()->bPlayerWon = true;
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** BoxBoxCollision between NPC and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE)
				{
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** BoxBoxCollision between Structure and Projectile ***" << endl;
					break;
				}
			}
		}
	}

	//check constraint in world
	
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// If the entity is not active, then skip it
		if ((*it)->GetStatus() == false)
			continue;

		if (((*it)->GetPosition().x >= 45.f || (*it)->GetPosition().x <= -45.f) ||
			((*it)->GetPosition().z >= 45.f || (*it)->GetPosition().z <= -45.f))
		{
			(*it)->SetPosition((*it)->GetPreviousPosition());
		}
	}

	if (bResult == true)
	{
		CCameraEffectsManager::GetInstance()->Get("BloodScreen")->SetStatus(true);
	}

	if (CGameManager3D::GetInstance()->bPlayerLost)
	{
		CCameraEffectsManager::GetInstance()->Get("LoseScreen")->SetStatus(true);
	}
	else if (CGameManager3D::GetInstance()->bPlayerWon)
	{
		CCameraEffectsManager::GetInstance()->Get("WinScreen")->SetStatus(true);
	}

	return true;
}

/**
 @brief CleanUp all CEntity3Ds which are marked for deletion
 */
void CSolidObjectManager::CleanUp(void)
{
	std::list<CSolidObject*>::iterator it, end;
	it = lSolidObject.begin();
	end = lSolidObject.end();
	while (it != end)
	{
		if ((*it)->IsToDelete())
		{
			// Delete the CSolidObject
			delete *it;
			// Go to the next iteration after erasing from the list
			it = lSolidObject.erase(it);
		}
		else
		{
			// Go to the next iteration
			++it;
		}
	}
}

/**
 @brief Render this class instance
 */
void CSolidObjectManager::Render(void)
{
	// Render all entities
	std::list<CSolidObject*>::iterator it, end;
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		(*it)->SetView(view);
		(*it)->SetProjection(projection);
		(*it)->PreRender();
		(*it)->Render();
		(*it)->PostRender();
	}
}

void CSolidObjectManager::SetEnemyCount(int newCount)
{
	enemyCount = newCount;
}

int CSolidObjectManager::GetEnemyCount(void)
{
	return enemyCount;
}
