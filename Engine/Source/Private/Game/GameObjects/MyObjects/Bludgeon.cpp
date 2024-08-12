#include "Game/GameObjects/MyObjects/Bludgeon.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSMaterial.h"
#include "Graphics/PModel.h"
#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PSCamera.h"

Bludgeon::Bludgeon()
{
	m_BludgeonOffset = glm::vec3(-25.0f, 0.0f, 50.0f);
	isSwinging = false;
}

void Bludgeon::SwingBludgeon()
{
	if (!isSwinging)
		isSwinging = true;
}

void Bludgeon::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Bludgeon/Bludgeon.fbx").lock())
	{
		TShared<PTexture> bludgeonTex = TMakeShared<PTexture>();
		TShared<PTexture> bludgeonSpecTex = TMakeShared<PTexture>();
		TShared<PTexture> bludgeonNormTex = TMakeShared<PTexture>();
		TShared<PSMaterial> bludgeonMat = TMakeShared<PSMaterial>();
		bludgeonTex->LoadTexture("Bludgeon base colour", "Models/Bludgeon/textures/Bludgeon_BaseColor.png");
		bludgeonSpecTex->LoadTexture("Bludgeon specular", "Models/Bludgeon/textures/Bludgeon_Specular.png");
		bludgeonNormTex->LoadTexture("Bludgeon normal", "Models/Bludgeon/textures/Bludgeon_Normal.png");
		bludgeonMat->m_BaseColourMap = bludgeonTex;
		bludgeonMat->m_SpecularMap = bludgeonSpecTex;
		bludgeonMat->m_NormalMap = bludgeonNormTex;
		modelRef->SetMaterialBySlot(0, bludgeonMat);
		modelRef->GetTransform().scale = glm::vec3(0.25f);
		GetTransform().position = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock()->transform.position + m_BludgeonOffset;
	}
}

void Bludgeon::OnTick(float deltaTime)
{
	if (const auto& camRef = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock())
	{

	}


	
}
