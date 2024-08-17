#include "Game/GameObjects/MyObjects/Torch.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSMaterial.h"
#include "Graphics/PModel.h"
#include "Graphics/PSLight.h"
#include "Graphics/PGraphicsEngine.h"

Torch::Torch()
{
}

void Torch::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Torch/torch.fbx").lock())
	{
		TShared<PTexture> tex = TMakeShared<PTexture>();
		TShared<PTexture> specTex = TMakeShared<PTexture>();
		TShared<PTexture> normTex = TMakeShared<PTexture>();
		TShared<PSMaterial> mat = TMakeShared < PSMaterial>();
		tex->LoadTexture("Torch base colour", "Models/Torch/LowResTex/torchColor.png");
		specTex->LoadTexture("Torch specular", "Models/Torch/LowResTex/torchSpecular.png");
		normTex->LoadTexture("Torch normal", "Models/Torch/LowResTex/torchNormal.png");
		mat->m_BaseColourMap = tex;
		mat->m_SpecularMap = specTex;
		mat->shininess = 16.0f;
		mat->specularStrength = 0.25f;
		mat->m_NormalMap = normTex;
		//specTex->LoadTexture("Torch specular", "Models/Torch/textures/torchSpecular.png");
		modelRef->SetMaterialBySlot(0, mat);
	}

	if (const auto& lightRef = PGameEngine::GetGameEngine()->GetGraphics()->CreatePointLight().lock())
	{
		if (GetTransform().rotation.y == 0)
			lightRef->position = GetTransform().position + glm::vec3(0.0f, 150.0f, 50.0f);
		else if (GetTransform().rotation.y == 180)
			lightRef->position = GetTransform().position + glm::vec3(0.0f, 150.0f, -50.0f);
		if (GetTransform().rotation.y == 90)
			lightRef->position = GetTransform().position + glm::vec3(50.0f, 150.0f, 0.0f);
		else if (GetTransform().rotation.y == -90)
			lightRef->position = GetTransform().position + glm::vec3(-50.0f, 150.0f, 0.0f);


		lightRef->colour = glm::vec3(1.0f, 0.5f, 0.0f);

		lightRef->linear = 0.00014;
		lightRef->quadratic = 0.000007f;
		lightRef->intensity = 1.0f;
	}

}
