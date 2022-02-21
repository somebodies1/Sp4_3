/**
 CAssaultRifle
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "AssaultRifle.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CAssaultRifle::CAssaultRifle(void)
{
}

/**
@brief Default Destructor
*/
CAssaultRifle::~CAssaultRifle(void)
{
}

/**
 @brief Initialise this instance to default values
 */
bool CAssaultRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 20;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 20;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 100;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 100;

	// The time between shots
	dTimeBetweenShots = 0.1111;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// The maximum elapsed time for reloading of a magazine in milliseconds
	dMaxReloadTime = 3.0f;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	// Call the parent's Init()
	CEntity3D::Init();
	weaponType = WEAPON_TYPE::ASSAULT_RIFLE;

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/AssaultRifle/assault_rifle.obj";
	cout << "Loaded: "<< file_path.c_str() << endl;
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/AssaultRifle/assault_rifle.obj" << endl;
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
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/AssaultRifle/assault_rifle2.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/AssaultRifle/assault_rifle.jpg" << endl;
		return false;
	}

	return true;
}
