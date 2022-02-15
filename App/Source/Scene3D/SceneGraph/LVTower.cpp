/**
 LVTower
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "LVTower.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/EntityManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
LVTower::LVTower(void)
{
}

/**
 @brief Destructor
 */
LVTower::~LVTower(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool LVTower::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(CSceneNode::LV, 0);
	cSceneNode->SetShader("Shader3D");
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(2.0f, -2.0f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(-5.0f, fCheckHeight + 0.5f, 5.0f));
	cSceneNode->Init();
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(CSceneNode::LV, 1);
	cSceneNodeChild->SetShader("Shader3D");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(0.2f), glm::vec3(0.0f, -1.0f, 0.0f));
	cSceneNode->AddChild(cSceneNodeChild);

	CEntityManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void LVTower::PrintSelf(void)
{
	cout << "LVTower::PrintSelf()" << endl;
}