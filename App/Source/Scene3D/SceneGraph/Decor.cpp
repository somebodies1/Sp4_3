/**
 Decor
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Decor.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/EntityManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
Decor::Decor(void)
{
}

/**
 @brief Destructor
 */
Decor::~Decor(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool Decor::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(CSceneNode::BANANA, 0);
	cSceneNode->SetShader("Shader3D");
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(2.0f, -2.0f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(10.0f, fCheckHeight + 4.0f, 3.0f));
	cSceneNode->Init();
	cSceneNode->bContinuousUpdate = true;
	//cSceneNode->SetUpdateRotateMtx(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(CSceneNode::BANANA, 1);
	cSceneNodeChild->SetShader("Shader3D");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(0.0f, -1.0f, 0.0f));
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(0.2f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode->AddChild(cSceneNodeChild);

	CSceneNode* cSceneNodeChild2 = new CSceneNode(CSceneNode::BANANA, 2);
	cSceneNodeChild2->SetShader("Shader3D");
	cSceneNodeChild2->Init();
	cSceneNodeChild2->SetWorldTranslateMtx(glm::vec3(0.0f, -1.0f, 0.0f));
	cSceneNodeChild2->bContinuousUpdate = true;
	cSceneNodeChild2->SetUpdateRotateMtx(glm::radians(0.2f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->AddChild(cSceneNodeChild2);

	CEntityManager::GetInstance()->Add(cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void Decor::PrintSelf(void)
{
	cout << "Decor::PrintSelf()" << endl;
}