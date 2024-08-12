#include "Game/GameObjects/MyObjects/Throne.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSMaterial.h"
#include "Graphics/PModel.h"

Throne::Throne()
{
	
}

void Throne::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Throne/Throne.fbx").lock())
	{
		// textures
		TShared<PTexture> throneTex = TMakeShared<PTexture>();
		TShared<PTexture> throneSpecTex = TMakeShared<PTexture>();
		TShared<PTexture> throneNormTex = TMakeShared<PTexture>();
		throneTex->LoadTexture("Throne base colour", "Models/Throne/textures/RustedThrone_Base_Color.png");
		throneSpecTex->LoadTexture("Throne specular", "Models/Throne/textures/RustedThrone_Specular.png");
		throneNormTex->LoadTexture("Throne normal", "Models/Throne/textures/RustedThrone_Normal_OpenGL.png");
		// materials
		TShared<PSMaterial> throneMat = TMakeShared<PSMaterial>();
		throneMat->m_BaseColourMap = throneTex;
		throneMat->m_SpecularMap = throneSpecTex;
		throneMat->m_NormalMap = throneNormTex;
		modelRef->SetMaterialBySlot(0, throneMat);
	}
	AddCollision({ GetTransform().position, glm::vec3(130.0f, 205.0f, 100.0f) });
	//SetLifeTime(5.0f);
}

void Throne::OnTick(float deltaTime)
{
	PWorldObject::OnTick(deltaTime);

	// GetTransform().position.y += 1.0f * deltaTime;
}

void Throne::OnOverlap(const TShared<PWorldObject>& other, const TShared<PSCollision>& otherCol)
{
	if (otherCol->type == PECollisionType::PLAYER)
	{
		PDebug::Log("Colliding with throne");
	}
}
