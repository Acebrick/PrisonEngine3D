#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PModel.h"
#include "Graphics/PShaderProgram.h"
#include "Math/PSTransform.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSCamera.h"
#include "Graphics/PSLight.h"

// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

// Test mesh for debug
TWeak<PModel> m_Throne;
TWeak<PModel> m_Dungeon;
TWeak<PModel> m_Bludgeon;
TWeak<PModel> m_Skull;
TWeak<PSPointLight> m_PointLight;

bool PGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	if (sdlWindow == nullptr)
	{
		PDebug::Log("SDL window was null", LT_ERROR);
		PDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	// Create an open gl context
	m_SDLGLContext = SDL_GL_CreateContext(sdlWindow);

	// Test if the context failed
	if (m_SDLGLContext == nullptr)
	{
		PDebug::Log("SDL failed to create gl context: " + std::string(SDL_GetError()), LT_ERROR);
		PDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	// Make the current context active for the SDL window
	// Test if it failed
	if (SDL_GL_MakeCurrent(sdlWindow, m_SDLGLContext) != 0)
	{
		PDebug::Log("SDL failed to make gl context current: " + std::string(SDL_GetError()), LT_ERROR);
		PDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	if (vsync)
	{
		// Try enable adaptive vsync and test if it failed
		if (SDL_GL_SetSwapInterval(-1) != 0)
		{
			// Try to enable standard vsync and test if it failed
			if (SDL_GL_SetSwapInterval(1) != 0)
			{
				PDebug::Log("Graphics engine failed to initialise vsync" + std::string(SDL_GetError()), LT_WARN);
				return false;
			}
		}
	}

	// Initialise glew
	GLenum glewResult = glewInit();

	// Test if glew failed
	if (glewResult != GLEW_OK)
	{
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewResult));
		PDebug::Log("Graphics engine failed to initialise GLEW: " + errorMsg);
		return false;
	}

	// Enable depth to be tested
	glEnable(GL_DEPTH_TEST);

	// Create the shader object
	m_Shader = TMakeShared<PShaderProgram>();

	// Attempt to initialise shdaer and test if failed
	if (!m_Shader->InitShader("Shaders/SimpleShader/SimpleShader.vertex", "Shaders/SimpleShader/SimpleShader.frag"))
	{
		PDebug::Log("Graphics engine failed to intialise due to shader failure");
		return false;
	}

	// Create the camera
	m_Camera = TMakeShared<PSCamera>();
	m_Camera->transform.position.y = 25.0f;
	m_Camera->transform.position.z = -50.0f;

	// DEBUG
	// THRONE
	m_Throne = ImportModel("Models/Throne/Throne.fbx");
	m_Throne.lock()->GetTransform().position.z = 200.0f;
	m_Throne.lock()->GetTransform().rotation.y = 180.0f;
	// textures
	TShared<PTexture> throneTex = TMakeShared<PTexture>();
	TShared<PTexture> throneSpecTex = TMakeShared<PTexture>();
	throneTex->LoadTexture("Throne base colour", "Models/Throne/textures/RustedThrone_Base_Color.png");
	throneSpecTex->LoadTexture("Throne specular", "Models/Throne/textures/RustedThrone_Specular.png");
	// materials
	TShared<PSMaterial> throneMat = TMakeShared<PSMaterial>();
	throneMat->m_BaseColourMap = throneTex;
	throneMat->m_SpecularMap = throneSpecTex;
	m_Throne.lock()->SetMaterialBySlot(0, throneMat);

	// DUNGEON
	//m_Dungeon = ImportModel("Models/Dungeon/modularDungeon.fbx");
	TShared<PTexture> dungeonTex = TMakeShared<PTexture>();
	TShared<PTexture> dungeonTex2 = TMakeShared<PTexture>();
	TShared<PTexture> dungeonTex3 = TMakeShared<PTexture>();
	TShared<PTexture> dungeonSpecTex = TMakeShared<PTexture>();
	TShared<PTexture> dungeonSpecTex2 = TMakeShared<PTexture>();
	TShared<PTexture> dungeonSpecTex3 = TMakeShared<PTexture>();
	TShared<PSMaterial> dungeonMat = CreateMaterial();
	TShared<PSMaterial> dungeonMat2 = CreateMaterial();
	TShared<PSMaterial> dungeonMat3 = CreateMaterial();
	dungeonTex->LoadTexture("Dungeon brick wall colour", "Models/Dungeon/textures/brickwallcolor.png");
	dungeonTex2->LoadTexture("Dungeon floor tiles colour", "Models/Dungeon/textures/floortilesColor.png");
	dungeonTex3->LoadTexture("Dungeon stone trim colour", "Models/Dungeon/textures/stonetrimColor.png");
	dungeonSpecTex->LoadTexture("Dungeon brick wall specular", "Models/Dungeon/textures/brickWallSpecular.png");
	dungeonSpecTex2->LoadTexture("Dungeon floor tiles specular", "Models/Dungeon/textures/floortilesSpecular.png");
	dungeonSpecTex3->LoadTexture("Dungeon stone trim specular", "Models/Dungeon/textures/stonetrimSpecular.png");
	dungeonMat->m_BaseColourMap = dungeonTex;
	dungeonMat->m_SpecularMap = dungeonSpecTex;
	dungeonMat->shininess = 100.0f;
	dungeonMat->specularStrength = 0.1f;
	dungeonMat2->m_BaseColourMap = dungeonTex2;
	dungeonMat2->m_SpecularMap = dungeonSpecTex2;
	dungeonMat2->shininess = 100.0f;
	dungeonMat2->specularStrength = 0.1f;
	dungeonMat3->m_BaseColourMap = dungeonTex3;
	dungeonMat3->m_SpecularMap = dungeonSpecTex3;
	dungeonMat3->shininess = 100.0f;
	dungeonMat3->specularStrength = 0.1f;
	//m_Dungeon.lock()->SetMaterialBySlot(0, dungeonMat);
	//m_Dungeon.lock()->SetMaterialBySlot(1, dungeonMat2);
	//m_Dungeon.lock()->SetMaterialBySlot(2, dungeonMat3);

	// SKULL
	m_Skull = ImportModel("Models/Skull/scene.gltf");
	m_Skull.lock()->GetTransform().position.y = 70.0f;
	m_Skull.lock()->GetTransform().position.z = 50.0f;
	m_Skull.lock()->GetTransform().scale = glm::vec3(10.0f);
	TShared<PTexture> skullTex = TMakeShared<PTexture>();
	TShared<PSMaterial> skullMat = CreateMaterial();
	skullTex->LoadTexture("Skull base colour", "Models/Skull/textures/defaultMat_baseColor.jpeg");
	skullMat->m_BaseColourMap = skullTex;
	m_Skull.lock()->SetMaterialBySlot(0, skullMat);

	// BLUDGEON
	m_Bludgeon = ImportModel("Models/Bludgeon/Bludgeon.fbx");
	m_Bludgeon.lock()->GetTransform().position.y = 50.0f;
	m_Bludgeon.lock()->GetTransform().position.z = 30.0f;
	TShared<PTexture> bludgeonTex = TMakeShared<PTexture>();
	TShared<PTexture> bludgeonSpecTex = TMakeShared<PTexture>();
	TShared<PSMaterial> bludgeonMat = CreateMaterial();
	bludgeonTex->LoadTexture("Bludgeon base colour", "Models/Bludgeon/textures/Bludgeon_BaseColor.png");
	bludgeonSpecTex->LoadTexture("Bludgeon specular", "Models/Bludgeon/textures/Bludgeon_Specular.png");
	bludgeonMat->m_BaseColourMap = bludgeonTex;
	bludgeonMat->m_SpecularMap = bludgeonSpecTex;
	m_Bludgeon.lock()->SetMaterialBySlot(0, bludgeonMat);

	// POINT LIGHT
	m_PointLight = CreatePointLight();
	m_PointLight.lock()->colour = glm::vec3(1.0f, 0.0f, 0.0f);

	// Making a second model
	//ImportModel("Models/Axe/scene.gltf").lock()->GetTransform().position = glm::vec3(0.0f, 15.0f, 0.0f);

	// Create the dir light
	const auto& dirLight = CreateDirLight();

	// Check if exists as a reference and change it
	if (const auto& lightRef = dirLight.lock())
	{
		lightRef->colour = glm::vec3(1.0f, 1.0f, 1.0f);
		lightRef->intensity = 0.1f;
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
	}

	// Log the success of the graphics engine
	PDebug::Log("Successfully initialised graphics engine", LT_SUCCESS);

	return true;
}

void PGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// SKULL TRANSLATION & ROTATION
	static float skullZDir = 1.0f;

	if (m_Skull.lock()->GetTransform().position.z > 220.0f) // Move backwards
	{
		skullZDir = -1.0f;
	}
	else if (m_Skull.lock()->GetTransform().position.z < 30) // Move forwards
	{
		skullZDir = 1.0f;
	}

	// Update the skulls position
	m_Skull.lock()->GetTransform().position.z += skullZDir;

	// Rotate
	if (skullZDir == 1.0f)
	{
		m_Skull.lock()->GetTransform().rotation.x += 1.5f;
		m_Skull.lock()->GetTransform().rotation.y += 0.75f;
		m_Skull.lock()->GetTransform().rotation.z += 1.0f;
	}
	// Reverse rotation to look cool
	else
	{
		m_Skull.lock()->GetTransform().rotation.x -= 1.25f;
		m_Skull.lock()->GetTransform().rotation.y -= 0.5f;
		m_Skull.lock()->GetTransform().rotation.z -= 0.75f;
	}

	// POINT LIGHT
	m_PointLight.lock()->position = m_Skull.lock()->GetTransform().position;

	// Activate the shader
	m_Shader->Activate();

	// Set the world transformations based on the camera
	m_Shader->SetWorldTransform(m_Camera);

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	for (const auto& modelRef : m_Models)
	{
		modelRef->Render(m_Shader, m_Lights);
	} 

	// Presented the frame to the window
	// Swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}

TWeak<PSPointLight> PGraphicsEngine::CreatePointLight()
{
	const auto& newLight = TMakeShared<PSPointLight>();
	m_Lights.push_back(newLight);

	return newLight;
}

TWeak<PSDirLight> PGraphicsEngine::CreateDirLight()
{
	const auto& newLight = TMakeShared<PSDirLight>();
	m_Lights.push_back(newLight);

	return newLight;
}

TWeak<PModel> PGraphicsEngine::ImportModel(const PString& path)
{
	const auto& newModel = TMakeShared<PModel>();
	newModel->ImportModel(path);
	m_Models.push_back(newModel);
	return newModel;
}

TShared<PSMaterial> PGraphicsEngine::CreateMaterial()
{
	return TMakeShared<PSMaterial>();
}
