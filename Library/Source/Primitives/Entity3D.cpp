/**
 CEntity3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Entity3D.h"

// Include ImageLoader
#include "..\System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CEntity3D::CEntity3D()
	: cSettings(NULL)
	, sShaderName("")
	, VAO(0)
	, VBO(0)
	, IBO(0)
	, mesh(NULL)
	, iTextureID(0)
	, model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, eType(OTHERS)
	, vec3Position(0.0f)
	, vec3Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, vec3PreviousPosition(0.0f)
	, vec3Scale(1.0f)
	, fRotationAngle(0.0f)
	, vec3RotationAxis(1.0f)
	, vec4Colour(1.0f)
	, fMovementSpeed(5.f)
	, bStatus(false)
	, bToDelete(false)
{
}

/**
@brief Default Destructor
*/
CEntity3D::~CEntity3D(void)
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cSettings = NULL;

	// Delete the rendering objects in the graphics card
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
}

/**
@brief Initialise this instance to default values
*/
bool CEntity3D::Init(void)
{
	// Get the handler to the CSettings Singleton
	cSettings = CSettings::GetInstance();

	bStatus = true;
	maxHP = currentHP = 100;
	IsInvis = false;
	//activepowerList;
	return true;
}


/**
 @brief Set the name of the shader to be used in this class instance
 @param _name The name of the Shader instance in the CShaderManager
 */
void CEntity3D::SetShader(const std::string& _name)
{
	this->sShaderName = _name;
}

bool CEntity3D::DamageCheck(const int damage)
{
	currentHP -= damage;
	if (currentHP <= 0) //if dead
	{
		SetStatus(false);
		return true;
	}
	//if not dead yet
	return false;
}

// Set methods
void CEntity3D::SetTextureID(const GLuint iTextureID)
{
	this->iTextureID = iTextureID;
}
void CEntity3D::SetType(const TYPE eType)
{
	this->eType = eType;
}
void CEntity3D::SetPosition(const glm::vec3 vec3Position)
{
	this->vec3Position = vec3Position;
}
void CEntity3D::SetPreviousPosition(const glm::vec3 vec3PreviousPosition)
{
	this->vec3PreviousPosition = vec3PreviousPosition;
}
void CEntity3D::SetFront(const glm::vec3 vec3Front)
{
	this->vec3Front = vec3Front;
}
void CEntity3D::SetScale(const glm::vec3 vec3Scale)
{
	this->vec3Scale = vec3Scale;
}

void CEntity3D::SetRotation(const float fRotationAngle, const glm::vec3 vec3RotationAxis)
{
	this->fRotationAngle = fRotationAngle;
	this->vec3RotationAxis = vec3RotationAxis;
}
void CEntity3D::SetColour(const glm::vec4 vec4Colour)
{
	this->vec4Colour = vec4Colour;
}
void CEntity3D::SetMovementSpeed(const float fMovementSpeed)
{
	this->fMovementSpeed = fMovementSpeed;
}
void CEntity3D::SetStatus(const bool bStatus)
{
	this->bStatus = bStatus;
}

void CEntity3D::SetMaxHP(const int newMaxHP)
{
	maxHP = currentHP = newMaxHP;
}

void CEntity3D::SetHP(int playerhp)
{
	//currentHP = playerhp;
}

void CEntity3D::SetInvis(bool isinvis)
{
	IsInvis = isinvis;
}

// Get methods
const GLuint CEntity3D::GetTextureID(void) const
{
	return iTextureID;
}
const CEntity3D::TYPE CEntity3D::GetType(void) const
{
	return eType;
}
const glm::vec3 CEntity3D::GetPosition(void) const
{
	return vec3Position;
}
const glm::vec3 CEntity3D::GetPreviousPosition(void) const
{
	return vec3PreviousPosition;
}
const glm::vec3 CEntity3D::GetFront(void) const
{
	return vec3Front;
}
const glm::vec3 CEntity3D::GetScale(void) const
{
	return vec3Scale;
}
const float CEntity3D::GetRotationAngle(void) const
{
	return fRotationAngle;
}
const glm::vec3 CEntity3D::GetRotationAxis(void) const
{
	return vec3RotationAxis;
}
const glm::vec4 CEntity3D::GetColour(void) const
{
	return vec4Colour;
}
const float CEntity3D::GetMovementSpeed(void) const
{
	return fMovementSpeed;
}
const bool CEntity3D::GetStatus(void) const
{
	return bStatus;
}

int CEntity3D::GetHP(void)
{
	return currentHP;
}

bool CEntity3D::GetInvis()
{
	return IsInvis;
}

// These methods are for marking this CEntity3D for deletion
/**
 @brief Mark this CEntity3D for deletion
 @param bToDelete A const bool variable which means this CEntity3D is to be deleted if true, else false 
 */
void CEntity3D::SetToDelete(const bool bToDelete)
{
	this->bToDelete = bToDelete;
}

/**
 @brief Check if this CEntity3D is to be deleted
 @return A bool value of true means this CEntity3D is to be deleted, else false
 */
const bool CEntity3D::IsToDelete(void) const
{
	return bToDelete;
}

/**
 @brief Store position for rollback
 */
void CEntity3D::StorePositionForRollback(void)
{
	vec3PreviousPosition = vec3Position;
}

/**
 @brief Rollback the position to the previous position
 */
void CEntity3D::RollbackPosition(void)
{
	vec3Position = vec3PreviousPosition;
}

void CEntity3D::PowerupUpdate(const double dElapsedTime)
{
	//std::cout << "pUp update"<< endl;
	for (int i = 0; i < activepowerList.size(); i++)
	{
		std::cout << "Time LEft: " << activepowerList[i]->getTimeLeft() << endl;
		switch (activepowerList[i]->Update(dElapsedTime))
		{
		case powerup::SPEED:
			if (activepowerList[i]->getTimeLeft() <= 0.f)
			{
				activepowerList[i]->setDead(true);
				this->SetMovementSpeed(5.f); //effect
				activepowerList.erase(activepowerList.begin());
				//std::cout << "Speed Dead" << endl;
				break;
			
		case powerup::INVINCIBLE:
			if (activepowerList[i]->getTimeLeft() <= 0.f)
			{
				activepowerList[i]->setDead(true);
				this->SetInvis(false);
				activepowerList.erase(activepowerList.begin());
			}
			break;
		case powerup::SLOWED:
			if (activepowerList[i]->getTimeLeft() <= 0.f)
			{
				activepowerList[i]->setDead(true);
				this->SetMovementSpeed(5.f);
				activepowerList.erase(activepowerList.begin());
			}
			break;
			}
		}

		//std::vector<powerup*> toDelete;
		//for (int i = 0; i < activepowerList.size(); i++)
		//{
		//	if (activepowerList[i]->getDead())
		//	{
		//		toDelete.push_back(activepowerList[i]);
		//	}
		//}
		//
		//for (int j = 0; j < toDelete.size(); ++j)
		//{
		//	switch (toDelete[j]->getpowertype())
		//	{
		//	case powerup::SPEED:
		//		
		//		
		//		break;
		//	case powerup::INVINCIBLE:
		//		break;
		//	case powerup::FIRERATE:
		//		break;
		//	}
		//}
		//
		//toDelete.clear();
	}
}

void CEntity3D::AddPowerup(CEntity3D* dude, powerup::POWERUPTYPE pType, float newTime)
{
	
	switch (pType)
	{
	case powerup::SPEED:
		if (AddPowerupTime(dude, pType, newTime))
		{
			return;
		}
		else
		{
			powerup* p = new powerup(pType, newTime); //create new powerup
			dude->activepowerList.push_back(p); //push to vector
			dude->SetMovementSpeed(dude->GetMovementSpeed() * 3.f); //effect
			//std::cout << this->GetMovementSpeed() << endl;
			//std::cout << "New SPeed" << endl;
		}
		break;
	case powerup::INVINCIBLE:
		if (AddPowerupTime(dude, pType, newTime))
		{
			return;
		}
		else
		{
			powerup* p = new powerup(pType, newTime); //create new powerup
			dude->activepowerList.push_back(p); //push to vector
			dude->SetInvis(true); //effect
			//std::cout << this->GetMovementSpeed() << endl;
			//std::cout << "New SPeed" << endl;
		}
		break;
	case powerup::SLOWED:
		if (AddPowerupTime(dude, pType, newTime))
		{
			return;
		}
		else
		{
			powerup* p = new powerup(pType, newTime); //create new powerup
			dude->activepowerList.push_back(p); //push to vector
			dude->SetMovementSpeed(dude->GetMovementSpeed() * 0.5f); //effect
			//std::cout << this->GetMovementSpeed() << endl;
			//std::cout << "New SPeed" << endl;
		}
	
		break;
	}
}

bool CEntity3D::AddPowerupTime(CEntity3D* dude, powerup::POWERUPTYPE pType, float newT)
{
	for (int i = 0; i < dude->activepowerList.size(); ++i)
	{
		if (dude->activepowerList[i]->getpowertype() == pType)
		{
			dude->activepowerList[i]->setTimeLeft(newT);
			std::cout << "Time Left: " << endl;
			return true;
		}
	}
	return false;
}

