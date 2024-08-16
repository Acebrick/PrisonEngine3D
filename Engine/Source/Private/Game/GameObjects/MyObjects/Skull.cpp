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
}

void Skull::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Skull/scene.gltf").lock())
	{
		TShared<PTexture> skullTex = TMakeShared<PTexture>();
		TShared<PSMaterial> skullMat = TMakeShared<PSMaterial>();
		TShared<PTexture> skullNormTex = TMakeShared<PTexture>();
		skullTex->LoadTexture("Skull base colour", "Models/Skull/textures/defaultMat_baseColor.jpeg");
		skullNormTex->LoadTexture("Skull normal", "Models/Skull/textures/defaultMat_normal.jpeg");
		skullMat->m_BaseColourMap = skullTex;
		skullMat->m_NormalMap = skullNormTex;
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

	if (const auto& colRef = AddCollision({ GetTransform().position, GetTransform().scale}).lock())
	{
		colRef->type = PECollisionType::ALL;
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
		if (isPatrolling)
		{
			// Check if the distance between the skull and the player is less than 400
			if (glm::distance(GetTransform().position, camRef->transform.position) < 400.0f)
			{
				isPatrolling = false;
			}

			PatrolSquare(deltaTime);
		}
		else if (isAlive)
		{
			ChasePlayer(deltaTime);
		}
		else
		{
			if (GetTransform().position.y >= 50.0f)
			{
				GetTransform().position.y -= 500.0f * deltaTime;
				GetTransform().rotation += 500.0f * deltaTime;
			}
			else
			{
				m_Eye.lock()->intensity = 0.0f;
				m_HoverLight.lock()->intensity = 0.0f;
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
	if (GetTransform().position.x >= 800.0f &&
		GetTransform().position.z >= 800.0f)
	{
		// Go to right of room
		skullXDir = -1.0f;
		skullZDir = 0.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 270.0f;
	}
	// Has the skull reached the top right corner 
	else if (GetTransform().position.x <= -800.0f &&
		GetTransform().position.z >= 800.0f)
	{
		// Go to back of room
		skullXDir = 0.0f;
		skullZDir += -1.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 180.0f;
	}
	// Has the skull reached the bottom right corner 
	else if (GetTransform().position.x <= -800.0f &&
		GetTransform().position.z <= -800.0f)
	{
		// Go to left of room
		skullXDir += 1.0f;
		skullZDir = 0.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 90.0f;
	}
	// Has the skull reached the bottom left corner 
	else if (GetTransform().position.x >= 800.0f &&
		GetTransform().position.z <= -800.0f)
	{
		// Go to front of room
		skullXDir = 0.0f;
		skullZDir += 1.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 0.0f;
	}

	// Translate skull
	GetTransform().position.x += skullXDir * movementSpeed * deltaTime;
	GetTransform().position.z += skullZDir * movementSpeed * deltaTime;

	
}

void Skull::ChasePlayer(float deltaTime)
{
	m_Eye.lock()->colour = m_HoverLight.lock()->colour = glm::vec3(1.0f, 0.0f, 0.0f);

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
