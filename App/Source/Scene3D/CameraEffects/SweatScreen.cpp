/**
 CSweatScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "SweatScreen.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CSweatScreen::CSweatScreen(void)
{
}

/**
 @brief Destructor
 */
CSweatScreen::~CSweatScreen(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CSweatScreen::Init(void)
{
	// Call the parent's Init()
	CCameraEffects::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate the mesh
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 2.0f, 2.0f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/CameraEffects_Sweat.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/CSweat.tga" << endl;
		return false;
	}

	return true;
}

bool CSweatScreen::Update(const double dElapsedTime)
{
	return true;
}
