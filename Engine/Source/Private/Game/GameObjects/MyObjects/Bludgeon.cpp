#include "Game/GameObjects/MyObjects/Bludgeon.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSMaterial.h"
#include "Graphics/PModel.h"
#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PSCamera.h"

Bludgeon::Bludgeon()
{
	m_BludgeonOffset = glm::vec3(-25.0f, 0.0f, 50.0f);
	m_IsSwinging = false;
	m_SwingTime = 1.0f;
}

void Bludgeon::ToggleIsSwinging()
{
	m_IsSwinging = true;
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
		modelRef->GetTransform().scale = glm::vec3(2.0f);
		GetTransform().position = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock()->transform.position + m_BludgeonOffset;
	}
}

void Bludgeon::OnTick(float deltaTime)
{
	if (const auto& camRef = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock())
	{
		if (!m_IsSwinging)
		{
			GetTransform().position.y = camRef->transform.position.y;
			GetTransform().rotation.x = 0.0f;
			GetTransform().rotation.z = 0.0f;
			m_StopSwingingTime = m_LifeTimeTimer + m_SwingTime;
		}
		
		else
		{		
			if (m_StopSwingingTime >= m_LifeTimeTimer)
			{
				GetTransform().position.y = camRef->transform.position.y - 50.0f;

				if (camRef->transform.Forward().z >= 0)
				{
					GetTransform().rotation.x = 90.0f;
					GetTransform().rotation.z = 90.0f * -camRef->transform.Forward().x;
				}
				else
				{
					GetTransform().rotation.x = -90.0f;
					GetTransform().rotation.z = 90.0f * -camRef->transform.Forward().x;
				}
			}
			else
				m_IsSwinging = false;
		}
	}
}