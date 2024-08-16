#include "Game/GameObjects/MyObjects/Dungeon.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSMaterial.h"
#include "Graphics/PModel.h"

void Dungeon::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Dungeon/modularDungeon.fbx").lock())
	{
		TShared<PTexture> dungeonTex = TMakeShared<PTexture>();
		TShared<PTexture> dungeonTex2 = TMakeShared<PTexture>();
		TShared<PTexture> dungeonTex3 = TMakeShared<PTexture>();
		TShared<PTexture> dungeonSpecTex = TMakeShared<PTexture>();
		TShared<PTexture> dungeonSpecTex2 = TMakeShared<PTexture>();
		TShared<PTexture> dungeonSpecTex3 = TMakeShared<PTexture>();
		TShared<PTexture> dungoenNormTex = TMakeShared<PTexture>();
		TShared<PTexture> dungoenNormTex2 = TMakeShared<PTexture>();
		TShared<PTexture> dungoenNormTex3 = TMakeShared<PTexture>();
		TShared<PSMaterial> dungeonMat = TMakeShared<PSMaterial>();
		TShared<PSMaterial> dungeonMat2 = TMakeShared<PSMaterial>();
		TShared<PSMaterial> dungeonMat3 = TMakeShared<PSMaterial>();
		dungeonTex->LoadTexture("Dungeon brick wall colour", "Models/Dungeon/LowResTex/brickwallcolor.png");
		dungeonTex2->LoadTexture("Dungeon floor tiles colour", "Models/Dungeon/LowResTex/floortilesColor.png");
		dungeonTex3->LoadTexture("Dungeon stone trim colour", "Models/Dungeon/LowResTex/stonetrimColor.png");
		dungeonSpecTex->LoadTexture("Dungeon brick wall specular", "Models/Dungeon/LowResTex/brickWallSpecular.png");
		dungeonSpecTex2->LoadTexture("Dungeon floor tiles specular", "Models/Dungeon/LowResTex/floortilesSpecular.png");
		dungeonSpecTex3->LoadTexture("Dungeon stone trim specular", "Models/Dungeon/LowResTex/stonetrimSpecular.png");
		dungoenNormTex->LoadTexture("Dungeon brick wall normal map", "Models/Dungeon/textures/brickwallNormal.png");
		dungoenNormTex2->LoadTexture("Dungeon floor tiles normal map", "Models/Dungeon/textures/floortilesNormal.png");
		dungoenNormTex3->LoadTexture("Dungeon stone trim normal map", "Models/Dungeon/textures/stonetrimNormal.png");
		dungeonMat->m_BaseColourMap = dungeonTex;
		dungeonMat->m_SpecularMap = dungeonSpecTex;
		dungeonMat->m_NormalMap = dungoenNormTex;
		dungeonMat->shininess = 100.0f;
		dungeonMat->specularStrength = 0.1f;
		dungeonMat2->m_BaseColourMap = dungeonTex2;
		dungeonMat2->m_SpecularMap = dungeonSpecTex2;
		dungeonMat->m_NormalMap = dungoenNormTex2;
		dungeonMat2->shininess = 100.0f;
		dungeonMat2->specularStrength = 0.1f;
		dungeonMat3->m_BaseColourMap = dungeonTex3;
		dungeonMat3->m_SpecularMap = dungeonSpecTex3;
		dungeonMat->m_NormalMap = dungoenNormTex3;
		dungeonMat3->shininess = 100.0f;
		dungeonMat3->specularStrength = 0.1f;
		modelRef->SetMaterialBySlot(0, dungeonMat);
		modelRef->SetMaterialBySlot(1, dungeonMat2);
		modelRef->SetMaterialBySlot(2, dungeonMat3);
		
	}

}
