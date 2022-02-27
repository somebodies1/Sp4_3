/**
 Soldier
 By: MengFanQi
 Date: Feb 2022
 */
#include "Soldier.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

// Set this to true if printing the debug info
#define _DEBUG_FSM false
/**
 @brief Default Constructor
 */
Soldier::Soldier(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, cPlayer3D(NULL)
	, cTerrain(NULL)
	, cWaypointManager(NULL)
	, cGameManager3D(NULL)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, +fHeightOffset, 0.0f);

	// Update the vectors for this enemy
	UpdateSoldierVectors();
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
Soldier::Soldier(const glm::vec3 vec3Position,
	const glm::vec3 vec3Front,
	const float fYaw,
	const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, cPlayer3D(NULL)
	, cTerrain(NULL)
	, cWaypointManager(NULL)
	, cGameManager3D(NULL)
{
	this->vec3Position = vec3Position;
	this->vec3Position.y += +fHeightOffset;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the vectors for this enemy
	UpdateSoldierVectors();
}

/**
 @brief Destructor
 */
Soldier::~Soldier(void)
{
	if (cWaypointManager)
	{
		// We set it to NULL only since it was declared somewhere else
		cWaypointManager = NULL;
	}

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}

	if (cPlayer3D)
	{
		// We set it to NULL only since it was declared somewhere else
		cPlayer3D = NULL;
	}

	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	if (cGameManager3D)
	{
		cGameManager3D = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool Soldier::Init(void)
{
	cGameManager3D = CGameManager3D::GetInstance();

	// Call the parent's Init()
	CSolidObject::Init();

	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\explode.ogg"), 31, true, false, false);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\scream.ogg"), 32, true, false, false);

	// Set the type
	SetType(CEntity3D::TYPE::NPC);

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/Enemy/soldier.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models Soldier" << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	iIndicesSize = index_buffer_data.size();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/Enemy/soldier.png", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load caveman png" << endl;
		return false;
	}

	// Store the handler to the terrain
	cTerrain = CTerrain::GetInstance();

	// Movement Control
	fMovementSpeed = 1.5f;
	iCurrentNumMovement = 0;
	iMaxNumMovement = 100;

	// Detection distance for player
	fDetectionDistance = 10.0f;

	// Init cWaypointManager
	cWaypointManager = new CWaypointManager;
	cWaypointManager->Init();

	// Add in some test Waypoints
	float fCheckHeight = cTerrain->GetHeight(0.0f, -30.0f);
	int m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(-80.0f, fCheckHeight, -30.0f));
	fCheckHeight = cTerrain->GetHeight(20.0f, -20.0f);
	m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(0.0f, fCheckHeight, 50.0f));
	fCheckHeight = cTerrain->GetHeight(-20.0f, -30.0f);
	m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(50.0f, fCheckHeight, 50.0f));

	//cWaypointManager->PrintSelf();

	//// Let the NPC face the nearest waypoint
	vec3Front = glm::normalize((cWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
	UpdateFrontAndYaw();

	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */	
void Soldier::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void Soldier::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void Soldier::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void Soldier::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool Soldier::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void Soldier::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		cSecondaryWeapon = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @return The CWeaponInfo* value
 */
CWeaponInfo* Soldier::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return cPrimaryWeapon;
	else if (iCurrentWeapon == 1)
		return cSecondaryWeapon;

	return NULL;
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void Soldier::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

/**
 @brief Discharge weapon
 @return A bool value
 */
bool Soldier::DischargeWeapon(void) const
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	//return NULL;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void Soldier::ProcessMovement(const SOLDIERMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime;
	if (direction == SOLDIERMOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == SOLDIERMOVEMENT::BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == SOLDIERMOVEMENT::LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == SOLDIERMOVEMENT::RIGHT)
		vec3Position += vec3Right * velocity;
	if (direction == SOLDIERMOVEMENT::RUSH)
		vec3Position += vec3Front * velocity * 2.f;

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
	}

	// Constraint the player's position
	Constraint();
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 */
void Soldier::ProcessRotate(const float fXOffset)
{
	// Update the yaw
	fYaw += fXOffset;// *fRotationSensitivity;

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdateSoldierVectors();
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool Soldier::Update(const double dElapsedTime)
{
	// Don't update if this entity is not active
	if (bStatus == false)
	{
		return false;
	}

	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();

	switch (sCurrentFSM)
	{
	case FSM::IDLE:
		if (iFSMCounter > iMaxFSMCounter)
		{
			sCurrentFSM = FSM::ATTACK;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Rested: Switching to Patrol State" << endl;
		}
		iFSMCounter++;
		break;
	//case FSM::PATROL:
	//	// Check if the destination position has been reached
	//	if (cWaypointManager->HasReachedWayPoint(vec3Position))
	//	{
	//		vec3Front = glm::normalize((cWaypointManager->GetNextWaypoint()->GetPosition() - vec3Position));
	//		UpdateFrontAndYaw();

	//		if (_DEBUG_FSM == true)
	//			cout << "Reached waypoint: Going to next waypoint" << endl;
	//	}
	//	else if (iFSMCounter > iMaxFSMCounter)
	//	{
	//		sCurrentFSM = FSM::IDLE;
	//		iFSMCounter = 0;
	//		if (_DEBUG_FSM == true)
	//			cout << "FSM Counter maxed out: Switching to Idle State" << endl;
	//	}
	//	else if (glm::distance(vec3Position, cPlayer3D->GetPosition()) < fDetectionDistance)
	//	{
	//		sCurrentFSM = FSM::ATTACK;
	//		iFSMCounter = 0;
	//		if (_DEBUG_FSM == true)
	//			cout << "Target found: Switching to Attack State" << endl;
	//	}
	//	else if (this->currentHP <= 50)
	//	{
	//		sCurrentFSM = FSM::SUICIDE;
	//		iFSMCounter = 0;
	//		if (_DEBUG_FSM == true)
	//			cout << "Target found: Switching to Run State" << endl;
	//	}
	//	else
	//	{
	//		// Process the movement
	//		ProcessMovement(SOLDIERMOVEMENT::FORWARD, (float)dElapsedTime);
	//		if (_DEBUG_FSM == true)
	//			cout << "Patrolling" << endl;
	//	}
	//	iFSMCounter++;
	//	break;
	case FSM::ATTACK:
	
			vec3Front = glm::normalize((cPlayer3D->GetPosition() - vec3Position));
			UpdateFrontAndYaw();

			// Discharge weapon
			if (DischargeWeapon() == false)
			{
				// Check if the weapon mag is empty
				if (cPrimaryWeapon->GetMagRound() == 0)
				{
					if (cPrimaryWeapon->GetTotalRound() != 0)
					{
						// Reload the weapon
						cPrimaryWeapon->Reload();
					}
				}
			}

			// Process the movement
			ProcessMovement(SOLDIERMOVEMENT::FORWARD, (float)dElapsedTime);
			if (_DEBUG_FSM == true)
				cout << "Attacking now" << endl;
		
		if (this->currentHP <= 50)
		{
			sCurrentFSM = FSM::SUICIDE;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Target found: Switching to Run State" << endl;
		}
		//else
		//{
		//	// If NPC loses track of player, then go back to the nearest waypoint
		//	vec3Front = glm::normalize((cWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
		//	UpdateFrontAndYaw();

		//	// Swtich to patrol mode
		//	sCurrentFSM = FSM::PATROL;
		//	iFSMCounter = 0;
		//	if (_DEBUG_FSM == true)
		//		cout << "Switching to Patrol State" << endl;
		//}
		iFSMCounter++;
		break;
	case FSM::SUICIDE:
		//sound_controller->PlaySoundByID(17);
		vec3Front = glm::normalize((cPlayer3D->GetPosition() - vec3Position));
		UpdateFrontAndYaw();

		// Process the movement
		ProcessMovement(SOLDIERMOVEMENT::RUSH, (float)dElapsedTime);
		cSoundController->PlaySoundByID(32);
	
		cout << iFSMCounter << endl;

		if (iFSMCounter >= 500 || this->currentHP == 0)
		{
			// exlpode
			cSoundController->PlaySoundByID(31);
			//this->currentHP = 0;
			iFSMCounter = 0;
			cGameManager3D->iAmtOfEnemies--;
			this->SetStatus(false);

			if (_DEBUG_FSM == true)
				cout << "Exploded" << endl;
		}
		iFSMCounter++;
		break;
	default:
		break;
	}

	// Update the model
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update the weapon's position
	if (cPrimaryWeapon)
	{
		//cPrimaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		cPrimaryWeapon->Update(dElapsedTime);
		glm::mat4 gunModel = model;
		gunModel = glm::rotate(gunModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		gunModel = glm::translate(gunModel, glm::vec3(0.05f, -0.075f, 0.5f));
		cPrimaryWeapon->SetModel(gunModel);
	}
	if (cSecondaryWeapon)
	{
		cSecondaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		cSecondaryWeapon->SetRotation(fYaw, glm::vec3(0.0f, 1.0f, 0.0f));
		cSecondaryWeapon->Update(dElapsedTime);
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void Soldier::PreRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void Soldier::Render(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::Render();

	cPrimaryWeapon->SetView(view);
	cPrimaryWeapon->SetProjection(projection);
	cPrimaryWeapon->PreRender();
	cPrimaryWeapon->Render();
	cPrimaryWeapon->PostRender();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void Soldier::PostRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PostRender();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void Soldier::UpdateSoldierVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);

	vec3Front = front;
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Right = vec3Right;
		cCamera->vec3Up = vec3Up;
	}
}

/**
 @brief Constraint the player's position
 */
void Soldier::Constraint(void)
{
	// Get the new height
	float fNewYValue = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	// Smooth out the change in height
	vec3Position.y += (fNewYValue - vec3Position.y) * 0.05f;
}

/**
 @brief Update Front Vector and Yaw
 */
void Soldier::UpdateFrontAndYaw(void)
{
	fYaw = glm::degrees(glm::acos(dot(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front)));
	if (cross(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front).y < 0.0f)
		fYaw *= -1;
}
