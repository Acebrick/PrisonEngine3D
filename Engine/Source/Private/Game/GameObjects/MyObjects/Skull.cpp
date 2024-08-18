#include "Game/GameObjects/MyObjects/Skull.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSMaterial.h"
#include "Graphics/PModel.h"
#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PSLight.h"
#include "Graphics/PSCamera.h"

Skull::Skull()
{
	m_HoverLightOffset = glm::vec3(0.0f, -250.0f, 0.0f);
	movementSpeed = 500.0f;
	isPatrolling = true;
	isAlive = true;
	isCircling = false;
	patrolPointA = glm::vec3(0.0f);
	patrolPointB = glm::vec3(0.0f);
	roomLength = 1600.0f;
}

void Skull::SetupPatrol(glm::vec3 patrolPointA, glm::vec3 patrolPointB, float xDir, float zDir)
{
	this->patrolPointA = patrolPointA;
	this->patrolPointB = patrolPointB;
	skullXDir = xDir;
	skullZDir = zDir;
}

void Skull::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Skull/scene.gltf").lock())
	{
		TShared<PTexture> skullTex = TMakeShared<PTexture>();
		TShared<PSMaterial> skullMat = TMakeShared<PSMaterial>();
		//TShared<PTexture> skullNormTex = TMakeShared<PTexture>();
		skullTex->LoadTexture("Skull base colour", "Models/Skull/LowResTex/defaultMat_baseColor.png");
		//skullNormTex->LoadTexture("Skull normal", "Models/Skull/LowResTex/defaultMat_normal.png");
		skullMat->m_BaseColourMap = skullTex;
		//skullMat->m_NormalMap = skullNormTex;
		skullMat->shininess = 1.0f;
		skullMat->specularStrength = 0.01f;
		modelRef->SetMaterialBySlot(0, skullMat);
		GetTransform().scale = glm::vec3(50.0f);
	}

	// Create lights
	m_Eye = PGameEngine::GetGameEngine()->GetGraphics()->CreateSpotLight();
	m_HoverLight = PGameEngine::GetGameEngine()->GetGraphics()->CreatePointLight();

	// Set the light values
	if (const auto& eye = m_Eye.lock())
	{
		eye->colour = glm::vec3(0.0f, 1.0f, 0.0f);
		eye->SetInnerCutOff(5.0f);
		eye->SetOuterCutOff(10.0f);
		eye->linear = 0.00014f;
		eye->quadratic = 0.00000007f;
	}
	if (const auto& hoverLight = m_HoverLight.lock())
	{
		hoverLight->colour = glm::vec3(0.0f, 1.0f, 0.0f);
		hoverLight->linear = 0.007f;
		hoverLight->quadratic = 0.000002f;
	}

	if (const auto& colRef = AddCollision({ GetTransform().position, GetTransform().scale }).lock())
	{
		colRef->type = PECollisionType::ENEMY;
	}
}

void Skull::OnTick(float deltaTime)
{
	PWorldObject::OnTick(deltaTime);

	if (const auto& hoverLight = m_HoverLight.lock())
	{
		hoverLight->position = GetTransform().position + m_HoverLightOffset;
	}

	if (const auto& eye = m_Eye.lock())
	{
		eye->position = GetTransform().position;
		eye->direction = GetTransform().Forward();
	}

	if (const auto& camRef = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock())
	{
		if (isPatrolling) // If skull is alive and patrolling
		{
			// Check if the distance between the skull and the player is less than 800
			if (glm::distance(GetTransform().position, camRef->transform.position) < 800.0f)
			{
				isPatrolling = false;
			}

			// Run the appropriate patrol function
			if (isCircling)
				PatrolSquare(deltaTime);
			else
				PatrolPoints(deltaTime);

			// Translate skull
			GetTransform().position.x += skullXDir * movementSpeed * deltaTime;
			GetTransform().position.z += skullZDir * movementSpeed * deltaTime;

		}
		else if (isAlive) // If skull is alive and player is found
		{
			ChasePlayer(deltaTime);
		}
		else // If skull has been killed
		{
			if (GetTransform().position.y >= 50.0f)
			{
				// Fall down to the ground
				GetTransform().position.y -= 500.0f * deltaTime;

				// Rotate on every axis
				GetTransform().rotation += 500.0f * deltaTime;
			}
		}
	}
}

void Skull::OnOverlap(const TShared<PWorldObject>& other, const TShared<PSCollision>& otherCol)
{
	if (otherCol->tag == "Bludgeon" && otherCol->type != PECollisionType::NONE)
	{
		isAlive = false;
	}
}

void Skull::PatrolSquare(float deltaTime)
{
	// Has the skull reached the top left corner
	if (GetTransform().position.x >= patrolPointA.x &&
		GetTransform().position.z >= patrolPointA.z)
	{
		// Go to right of room
		skullXDir = -1.0f;
		skullZDir = 0.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 270.0f;
	}
	// Has the skull reached the top right corner 
	else if (GetTransform().position.x <= patrolPointA.x - roomLength &&
		GetTransform().position.z >= patrolPointA.z)
	{
		// Go to back of room
		skullXDir = 0.0f;
		skullZDir = -1.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 180.0f;
	}
	// Has the skull reached the bottom right corner 
	else if (GetTransform().position.x <= patrolPointA.x - roomLength &&
		GetTransform().position.z <= patrolPointA.z - roomLength)
	{
		// Go to left of room
		skullXDir = 1.0f;
		skullZDir = 0.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 90.0f;
	}
	// Has the skull reached the bottom left corner 
	else if (GetTransform().position.x >= patrolPointA.x &&
		GetTransform().position.z <= patrolPointA.z - roomLength)
	{
		// Go to front of room
		skullXDir = 0.0f;
		skullZDir = 1.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 0.0f;
	}
}

void Skull::ChasePlayer(float deltaTime)
{
	m_Eye.lock()->colour = glm::vec3(1.0f, 0.0f, 0.0f);
	m_HoverLight.lock()->colour = glm::vec3(1.0f, 0.0f, 0.0f);

	if (const auto& camRef = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock())
	{
		// Chase the player
		glm::vec3 distance = GetTransform().position - camRef->transform.position;
		GetTransform().position.x -= glm::normalize(distance).x * movementSpeed * deltaTime;
		GetTransform().position.z -= glm::normalize(distance).z * movementSpeed * deltaTime;

		if ((glm::normalize(distance).x > 0 && (glm::normalize(distance).z > 0) || (glm::normalize(distance).x > 0 && glm::normalize(distance).z < 0)))
		{
			GetTransform().rotation.y = -(glm::normalize(distance).z * 90.0f + 90.0f);
		}
		else
			GetTransform().rotation.y = (glm::normalize(distance).z * 90.0f + 90.0f);
	}
}

void Skull::PatrolPoints(float deltaTime)
{
	// Note: These conditions can't use == as they will always run on only one axis

	// Check if the skull has moved beyond the patrol points on the x coord
	if (GetTransform().position.x < patrolPointA.x && GetTransform().position.x < patrolPointA.x)
	{
		skullXDir = 1.0f;
		GetTransform().rotation.y = 90.0f;
	}
	else if (GetTransform().position.x > patrolPointA.x && GetTransform().position.x > patrolPointB.x)
	{
		skullXDir = -1.0f;
		GetTransform().rotation.y = 270.0f;
	}

	// Check if the skull has moved beyond the patrol points on the x coord
	if (GetTransform().position.z < patrolPointA.z && GetTransform().position.z < patrolPointB.z)
	{
		skullZDir = 1.0f;
		GetTransform().rotation.y = 0.0f;
	}
	else if (GetTransform().position.z > patrolPointA.z && GetTransform().position.z > patrolPointB.z)
	{
		skullZDir = -1.0f;
		GetTransform().rotation.y = 180.0f;
	}
}