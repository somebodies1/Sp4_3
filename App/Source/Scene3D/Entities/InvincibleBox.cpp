/**
 CInvincibleBox
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "InvincibleBox.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CInvincibleBox::CInvincibleBox(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, fHeightOffset, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CInvincibleBox::CInvincibleBox(	const glm::vec3 vec3Position,
							const glm::vec3 vec3Front)
{
	this->vec3Position = glm::vec3(vec3Position.x, vec3Position.y + fHeightOffset, vec3Position.z);
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CInvincibleBox::~CInvincibleBox(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CInvincibleBox::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::INVINCIBLE_UP);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mesh = CMeshBuilder::GenerateBox(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Invince.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Invince.tga" << endl;
		return false;
	}

	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CInvincibleBox::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CInvincibleBox::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CInvincibleBox::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CInvincibleBox::Update(const double dElapsedTime)
{
    if (bStatus == false)
        return false;

	CSolidObject::Update(dElapsedTime);

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CInvincibleBox::PreRender(void)
{
    if (bStatus == false)
        return;
	CSolidObject::PreRender();
}

/**
@brief Render Render this instance
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CInvincibleBox::Render(void)
{
    if (bStatus == false)
        return;
	CSolidObject::Render();
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CInvincibleBox::PostRender(void)
{
    if (bStatus == false)
        return;
	CSolidObject::PostRender();
}
