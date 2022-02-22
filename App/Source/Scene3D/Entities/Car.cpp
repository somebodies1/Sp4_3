/**
 CHut_Concrete
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Car.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include CPlayer3D
#include "../Entities/Player3D.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCar::CCar(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, fHeightOffset, 0.0f);
	player = nullptr;
	player = CPlayer3D::GetInstance();
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 */
CCar::CCar(const glm::vec3 vec3Position,
	const glm::vec3 vec3Front)
{
	this->vec3Position = glm::vec3(vec3Position.x, vec3Position.y + fHeightOffset, vec3Position.z);
	this->vec3Front = vec3Front;
	player = CPlayer3D::GetInstance();
	carCam = CCamera::GetInstance();
	CarVector = Vector3(vec3Position.x, vec3Position.y, vec3Position.z);
}

/**
 @brief Destructor
 */
CCar::~CCar(void)
{
	player = nullptr;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCar::Init(void)
{
	accerleration = 15;
	currentMPH = 0;
	CarRot = 90;

	carTrans = carRot = 0;
	carTransDir = carRotDir = 1;

	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::STRUCTURE);

	if (m_bLODStatus == false)
	{
		if (LoadModelAndTexture("Models/van/van.obj",
			"Models/Car/vanTex.tga",
			VAO, iTextureID, iIndicesSize) == false)
		{
			cout << "Unable to load model and texture" << endl;
		}
	}
	else
	{
		if (LoadModelAndTexture("Models/van/van.obj",
			"Models/van/vanTex.tga",
			arrVAO[0], arriTextureID[0], arrIndicesSize[0]) == false)
		{
			cout << "Unable to load high LOD model and texture" << endl;
		}
		if (LoadModelAndTexture("Models/van/van.obj",
			"Models/van/vanTex_M.tga",
			arrVAO[1], arriTextureID[1], arrIndicesSize[1]) == false)
		{
			cout << "Unable to load mid LOD model and texture" << endl;
		}
		if (LoadModelAndTexture("Models/van/van_L.obj",
			"Models/van/vanTex_L.tga",
			arrVAO[2], arriTextureID[2], arrIndicesSize[2]) == false)
		{
			cout << "Unable to load low LOD model and texture" << endl;
		}
		//if (LoadModelAndTexture("Models/Donkey/Donkey.obj",
		//	"Models/Donkey/Donkey.png",
		//	arrVAO[0], arriTextureID[0], arrIndicesSize[0]) == false)
		//{
		//	cout << "Unable to load high LOD model and texture" << endl;
		//}
		//if (LoadModelAndTexture("Models/Donkey/DonkeyNoEarNoTail.obj",
		//	"Models/Donkey/DonkeyM.png",
		//	arrVAO[1], arriTextureID[1], arrIndicesSize[1]) == false)
		//{
		//	cout << "Unable to load mid LOD model and texture" << endl;
		//}
		//if (LoadModelAndTexture("Models/Donkey/DonkeyFarAway.obj",
		//	"Models/Donkey/DonkeyL.png",
		//	arrVAO[2], arriTextureID[2], arrIndicesSize[2]) == false)
		//{
		//	cout << "Unable to load low LOD model and texture" << endl;
		//}
	}

	return true;
}

/**
 @brief Load a model and texture
 @param filenameModel A const char* variable containing the filename for the model
 @param filenameTexture A const char* variable containing the filename for the texture
 @param VAO A GLuint& variable containing the VAO to be returned
 @param iTextureID A GLuint& variable containing the iTextureID to be returned
 @param iIndicesSize A GLuint& variable containing the iIndicesSize to be returned
 @return A bool variable
 */
bool CCar::LoadModelAndTexture(const char* filenameModel,
	const char* filenameTexture,
	GLuint& VAO,
	GLuint& iTextureID,
	GLuint& iIndicesSize)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = filenameModel;
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load " << filenameModel << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

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
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID(filenameTexture, false);
	if (iTextureID == 0)
	{
		cout << "Unable to load " << filenameTexture << endl;
		return false;
	}

	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CCar::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CCar::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CCar::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CCar::Update(const double dElapsedTime)
{
	CSolidObject::Update(dElapsedTime);

	if (m_bLODStatus)
	{
		float fDistanceToPlayer = glm::distance(vec3Position, CPlayer3D::GetInstance()->GetPosition());
		if (fDistanceToPlayer > arrLODDistance[LOW_DETAILS])
			eDetailLevel = LOW_DETAILS;
		else if (fDistanceToPlayer > arrLODDistance[MID_DETAILS])
			eDetailLevel = MID_DETAILS;
		else
			eDetailLevel = HIGH_DETAILS;
	}

	updateCar(dElapsedTime);

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCar::PreRender(void)
{
	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CCar::Render(void)
{
	if (m_bLODStatus == false)
	{
		// note: currently we set the projection matrix each frame, but since the projection 
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->activeShader->setMat4("view", view);
		CShaderManager::GetInstance()->activeShader->setMat4("model", model);

		// bind textures on corresponding texture units
		glBindTexture(GL_TEXTURE_2D, iTextureID);
		glActiveTexture(GL_TEXTURE0);
		// Render the mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		// note: currently we set the projection matrix each frame, but since the projection 
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->activeShader->setMat4("view", view);
		CShaderManager::GetInstance()->activeShader->setMat4("model", model);

		// bind textures on corresponding texture units
		glBindTexture(GL_TEXTURE_2D, arriTextureID[eDetailLevel]);
		glActiveTexture(GL_TEXTURE0);
		// Render the mesh
		glBindVertexArray(arrVAO[eDetailLevel]);
		glDrawElements(GL_TRIANGLES, arrIndicesSize[eDetailLevel], GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CCar::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Print Self
 */
void CCar::PrintSelf(void)
{
	cout << "CHut_Concrete::PrintSelf()" << endl;
	cout << "========================" << endl;
}

float CCar::GetAngle(Vector3 vec1, Vector3 vec2) {
	float dot_product = vec1.Dot(vec2);
	float length = vec1.Length() * vec2.Length();

	float angle = acos(dot_product / length);
	return Math::RadianToDegree(angle);
}

void CCar::updateCar(float dt)
{
	StorePositionForRollback();

	float spd = 0.5 * dt;
	float rotationspd = 7 * dt;

	//Entering car for player
	static float buttonPress = 1.0f;
	float length = glm::length(player->GetPosition() - vec3Position);
	if (length <= 3 && CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_M) && buttonPress <= 0.0f) {
		//justpress = !justpress;
		
		buttonPress = 1.f;
		player->SetSMovement(CPlayer3D::PLAYER_SMOVEMENT::CAR);
		std::cout << "Im in a car\n";
		AttachCamera(CCamera::GetInstance());
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_M) && buttonPress <= 0.0f)
	{
	buttonPress = 1.f;
	if (player->GetSMovement() == CPlayer3D::PLAYER_SMOVEMENT::CAR) {
		player->SetSMovement(CPlayer3D::PLAYER_SMOVEMENT::STAND);
		AttachCamera(nullptr);
	}
	}
	else if (buttonPress > 0.0f)
	{
	buttonPress -= dt;
	}



	if (player->GetSMovement() != CPlayer3D::PLAYER_SMOVEMENT::CAR)
	{
		float PosClamp = 0.25f * float(carTransDir);

		if (carTrans != PosClamp) {
			IncrementToValue(carTrans, spd, PosClamp);
		}
		else {
			carTransDir *= -1;
		}

		//Rotation
		float RotClamp = 2.5 * (float)carRotDir;

		if (carRot != RotClamp) {
			IncrementToValue(carRot, rotationspd, RotClamp);
		}
		else {
			carRotDir *= -1;
		}
	}
	else
	{
		IncrementToValue(carTrans, spd, 0);
		IncrementToValue(carRot, rotationspd, 0);

		carTransDir = 1;
		carRotDir = 1;
	}

	if (player->GetSMovement() != CPlayer3D::PLAYER_SMOVEMENT::CAR)
	{
		IncrementToValue(currentMPH, accerleration * dt, 0);


		if (currentMPH > 0) {
			Vector3 CarDir;
			Mtx44 rotation;
			rotation.SetToRotation(CarRot, 0, 1, 0);
			CarDir = (rotation * Vector3(0, 0, 1)).Normalized();

			CarVector += CarDir * currentMPH * dt;
			vec3Position = glm::vec3(CarVector.x, CarVector.y, CarVector.z);
		}
		return;
	}

	float acceleration_clamp = 30.f;
	float rotation_spd = 150 * (currentMPH / acceleration_clamp) * dt;

	bool moving = false;

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W)) {
		IncrementToValue(currentMPH, accerleration * dt, acceleration_clamp);
		moving = true;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S)) {
		IncrementToValue(currentMPH, accerleration * dt, -acceleration_clamp);
		moving = true;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
		CarRot += rotation_spd;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {
		CarRot -= rotation_spd;
	}

	if (!moving) {
		IncrementToValue(currentMPH, accerleration * dt, 0);
	}

	if (currentMPH != 0) {
		Vector3 CarDir;
		Mtx44 rotation;
		rotation.SetToRotation(CarRot, 0, 1, 0);
		CarDir = (rotation * Vector3(0, 0, 1)).Normalized();

		CarVector += CarDir * currentMPH * dt;
	}

	if ((CarVector.x >= 45.f || CarVector.x <= -45.f) ||
		(CarVector.z >= 45.f || CarVector.z <= -45.f))
		{
		this->RollbackPosition(); //Fixed world limit for car but it just cant move afterwards if you touch the world border
	}
	else
	{
		vec3Position = glm::vec3(CarVector.x, CarVector.y, CarVector.z);
		float fHeightCheck = CTerrain::GetInstance()->GetHeight(vec3Position.x, vec3Position.z);
		vec3Position.y = fHeightCheck + 0.5f;

		player->SetPosition(vec3Position);
		std::cout << "Player posx: " << player->GetPosition().x << endl;
		std::cout << "Player posy: " << player->GetPosition().y << endl;
		std::cout << "Player posz: " << player->GetPosition().z << endl;

		std::cout << "Car posx: " << vec3Position.x << endl;
		std::cout << "Car posy: " << vec3Position.y << endl;
		std::cout << "Car posz: " << vec3Position.z << endl;
	}
	

	if (carCam)
	{
		carCam->vec3Position.x = vec3Position.x + carCam->vec3Offset.x;
		carCam->vec3Position.y = vec3Position.y * 1.2f + carCam->vec3Offset.y;
		carCam->vec3Position.z = vec3Position.z + carCam->vec3Offset.z;
		carCam->vec3Front = vec3Front;
		carCam->vec3Up = glm::vec3(0.0f, 1.0f, 0.0f);
		//cCamera->vec3Right = vec3Right;
		//cCamera->fYaw = fYaw;
		//cCamera->fPitch = fPitch;
	}
}

void CCar::IncrementToValue(float& dir, float spd, float target)
{
	if (dir > target + spd) {
		dir -= spd;
	}
	else if (dir < target - spd) {
		dir += spd;
	}
	else {
		dir = target;
	}
}

void CCar::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->carCam = cCamera;

	// Update the camera's attributes with the player's attributes
	if (carCam)
	{
		carCam->vec3Position = vec3Position;
		carCam->vec3Front = vec3Front;
		//cCamera->vec3Up = vec3Up;
		//cCamera->vec3Right = vec3Right;
		//cCamera->fYaw = fYaw;
		//cCamera->fPitch = fPitch;
	}
}

bool CCar::IsCameraAttached(void)
{
	if (carCam)
		return true;
	return false;
}

