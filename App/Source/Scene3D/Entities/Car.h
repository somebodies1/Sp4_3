/**
 CHut_Concrete
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

 // Include CSolidObject
#include "SolidObject.h"

// Include CLevelOfDetails
#include "Primitives/LevelOfDetails.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "../Math/Vector3.h"


// Include Camera
#include "../Camera.h"

#include <string>

#include "Player3D.h"

#include "../Math/Mtx44.h"

#include "Inputs\KeyboardController.h"
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CCar : public CSolidObject, public CLevelOfDetails
{
public:

	float accerleration;
	float currentMPH;


	float CarRot;
	float carTrans;
	int carTransDir;
	float carRot;
	int carRotDir;

	//Car position
	Vector3 CarVector;
	Vector3 prev_CarVector;

	bool justpress = false;

	CPlayer3D* player;

	// Default Constructor
	CCar(void);

	//Carructor with vectors
	CCar(const glm::vec3 vec3Position,
		const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~CCar(void);

	// Initialise this class instance
	bool Init(void);

	// Load a model and texture
	bool LoadModelAndTexture(const char* filenameModel,
		const char* filenameTexture,
		GLuint& VAO,
		GLuint& iTextureID,
		GLuint& iIndicesSize);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	virtual void PrintSelf(void);

	float GetAngle(Vector3 vec1, Vector3 vec2);
	void updateCar(float dt);
	void IncrementToValue(float& dir, float spd, float target);
};
