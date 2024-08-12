#include "Game/GameObjects/MyObjects/Skull.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSMaterial.h"
#include "Graphics/PModel.h"
#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PSLight.h"

Skull::Skull()
{
	m_HoverLightOffset = glm::vec3(0.0f, -250.0f, 0.0f);
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
	}

	// Create lights
	m_LeftEye = PGameEngine::GetGameEngine()->GetGraphics()->CreateSpotLight();
	m_RightEye = PGameEngine::GetGameEngine()->GetGraphics()->CreateSpotLight();
	m_HoverLight = PGameEngine::GetGameEngine()->GetGraphics()->CreatePointLight();

	// Set the light values
	if (const auto& leftEye = m_LeftEye.lock())
	{
		if (const auto& rightEye = m_RightEye.lock())
		{
			rightEye->colour = leftEye->colour = glm::vec3(0.0f, 1.0f, 0.0f);
			leftEye->SetInnerCutOff(3.5f);
			rightEye->SetInnerCutOff(3.5f);	
			leftEye->SetOuterCutOff(5.0f);
			rightEye->SetOuterCutOff(5.0f);
			rightEye->linear = leftEye->linear = 0.00014f;
			rightEye->quadratic = leftEye->quadratic = 0.00000007f;
		}
	}
	if (const auto& hoverLight = m_HoverLight.lock())
	{
		hoverLight->colour = glm::vec3(0.0f, 1.0f, 0.0f);
		hoverLight->linear = 0.007f;
		hoverLight->quadratic = 0.000002f;
	}
}

void Skull::OnTick(float deltaTime)
{
	PWorldObject::OnTick(deltaTime);
	
	// Has the skull reached the top left corner
	if (GetTransform().position.x >= 800.0f &&
		GetTransform().position.z >= 800.0f)
	{
		// Go to right of room
		skullXDir = -1.0f;
		skullZDir = 0.0f;
		// Rotate to face moving direction
		GetTransform().rotation.y = 270.0f;
		// Set move direcion
		movingOnX = true;
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
		// Set move direcion
		movingOnX = false;
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
		// Set move direcion
		movingOnX = true;
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
		// Set move direcion
		movingOnX = false;
	}

	// Translate skull
	GetTransform().position.x += skullXDir * movementSpeed * deltaTime;
	GetTransform().position.z += skullZDir * movementSpeed * deltaTime;
	
	if (const auto& hoverLight = m_HoverLight.lock())
	{
		hoverLight->position = GetTransform().position + m_HoverLightOffset;
	}

	if (const auto& leftEye = m_LeftEye.lock())
	{
		if (const auto& rightEye = m_RightEye.lock())
		{
			if (movingOnX)
			{
				leftEye->position.x = GetTransform().position.x;
				leftEye->position.y = GetTransform().position.y + 1;
				leftEye->position.z = GetTransform().position.z - 3;
				leftEye->direction.x = GetTransform().Forward().x;
				leftEye->direction.y = GetTransform().Forward().y;
				leftEye->direction.z = GetTransform().Forward().z - 0.03f;
				rightEye->position.x = GetTransform().position.x;
				rightEye->position.y = GetTransform().position.y + 1;
				rightEye->position.z = GetTransform().position.z + 3;
				rightEye->direction.x = GetTransform().Forward().x;
				rightEye->direction.y = GetTransform().Forward().y;
				rightEye->direction.z = GetTransform().Forward().z + 0.03f;
			}
			else
			{
				leftEye->position.x = GetTransform().position.x - 3;
				leftEye->position.y = GetTransform().position.y + 1;
				leftEye->position.z = GetTransform().position.z;
				leftEye->direction.x = GetTransform().Forward().x - 0.03f;
				leftEye->direction.y = GetTransform().Forward().y;
				leftEye->direction.z = GetTransform().Forward().z;
				rightEye->position.x = GetTransform().position.x + 3;
				rightEye->position.y = GetTransform().position.y + 1;
				rightEye->position.z = GetTransform().position.z;
				rightEye->direction.x = GetTransform().Forward().x + 0.03f;
				rightEye->direction.y = GetTransform().Forward().y;
				rightEye->direction.z = GetTransform().Forward().z;
			}
		}
	}

	//GetTransform().position.x += 1.0f * deltaTime;
}

void Skull::CircleRoom()
{
}
