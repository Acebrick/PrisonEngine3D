#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PModel.h"
#include "Graphics/PShaderProgram.h"
#include "Math/PSTransform.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSCamera.h"
#include "Graphics/PSLight.h"
#include "Math/PSCollision.h"

// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


const std::vector<PSVertexData> colMeshVData = {
	// x   // y   // z 
{ {	-1.0f, -1.0f,  1.0f } }, // bl f
{ {	 1.0f, -1.0f,  1.0f } }, // br f
{ {  1.0f,  1.0f,  1.0f } }, // tr f
{ { -1.0f,  1.0f,  1.0f } }, // tl f
{ {	-1.0f, -1.0f, -1.0f } }, // bl b
{ {	 1.0f, -1.0f, -1.0f } }, // br b
{ {  1.0f,  1.0f, -1.0f } }, // tr b
{ { -1.0f,  1.0f, -1.0f } }, // tl b
};

const std::vector<uint32_t> colMeshIData = {
	0, 1, 1, 2, 2, 3, 3, 0, // front
	4, 5, 5, 6, 6, 7, 7, 4, // back
	0, 4, 1, 5, 2, 6, 3, 7  // sides
};

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

	// Create the shader object
	m_WireShader = TMakeShared<PShaderProgram>();

	// Attempt to initialise shdaer and test if failed
	if (!m_WireShader->InitShader("Shaders/Wireframe/Wireframe.vertex", "Shaders/Wireframe/Wireframe.frag"))
	{
		PDebug::Log("Graphics engine failed to intialise due to shader failure");
		return false;
	}

	// Create the camera
	m_Camera = TMakeShared<PSCamera>();
	m_Camera->transform.position = glm::vec3(5800.0f, 350.0f, -2800.0f);

	//TShared<PTexture> defaultTexture = TMakeShared<PTexture>();

	//if (!defaultTexture->LoadTexture("Default grid", "Textures/P_DefaultGrid.png"))
	//{
	//	PDebug::Log("Graphics engine default texture failed to load", LT_ERROR);
	//}

	//m_DefaultMaterial = TMakeShared<PSMaterial>();

	//m_DefaultMaterial->m_BaseColourMap = defaultTexture;

	
	// FLASHLIGHT
	//m_Flashlight = CreateSpotLight();
	//if (const auto& lightRef = m_Flashlight.lock())
	//{
	//	m_Flashlight.lock()->linear = 0.0014f;
	//	m_Flashlight.lock()->quadratic = 0.000007f;
	//}

	// NORMAL MAPPING TEST MODEL
	//m_Wall = ImportModel("Models/Wall/scene.gltf");
	//TShared<PTexture> wallTex = TMakeShared<PTexture>();
	//TShared<PTexture> wallNormal = TMakeShared<PTexture>();
	//TShared<PSMaterial> wallMat = TMakeShared<PSMaterial>();
	//wallTex->LoadTexture("Wall diffuse", "Models/Wall/textures/Brick_Wall_02_baseColor.jpeg");
	//wallNormal->LoadTexture("Wall normal", "Models/Wall/textures/Brick_Wall_02_normal.png");
	//m_Wall.lock()->GetTransform().position.x = -400.0f;
	//m_Wall.lock()->GetTransform().position.y = 20.0f;
	//m_Wall.lock()->GetTransform().position.z = -50.0f;
	//wallMat->m_BaseColourMap = wallTex;
	//wallMat->m_NormalMap = wallNormal;
	//m_Wall.lock()->SetMaterialBySlot(0, wallMat);

	// Create the dir light
	const auto& dirLight = CreateDirLight();

	// Check if exists as a reference and change it
	if (const auto& lightRef = dirLight.lock())
	{
		lightRef->colour = glm::vec3(1.0f, 0.0f, 0.0f);
		lightRef->intensity = 0.1f;
		lightRef->direction = glm::vec3(0.0f, 1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.01f);

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

	// Activate the shader
	m_Shader->Activate();

	// Set the world transformations based on the camera
	m_Shader->SetWorldTransform(m_Camera);

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	for (int i = m_Models.size() - 1; i >= 0; --i)
	{
		// Checking if the reference still exists
		if (const auto& modelRef = m_Models[i].lock())
			// Render if there is a reference
			modelRef->Render(m_Shader, m_Lights);
		else
			// Erasing from the array if there is no reference
			m_Models.erase(m_Models.begin() + i);
	} 

	// WIREFRAME SHADER
	// Activate the shader
	m_WireShader->Activate();

	// Set the world transformations based on the camera
	m_WireShader->SetWorldTransform(m_Camera);

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	for (int i = m_Collisions.size() - 1; i >= 0; --i)
	{
		// Checking if the reference still exists
		if (const auto& colRef = m_Collisions[i].lock())
		{
			PSTransform transform;
			transform.position = colRef->box.position;
			transform.scale = colRef->box.halfSize;

			// Render if there is a reference
			colRef->debugMesh->WireRender(m_WireShader, transform);
		}
		else
		{
			// Erasing from the array if there is no reference
			m_Collisions.erase(m_Collisions.begin() + i);
		}
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

TWeak<PSSpotLight> PGraphicsEngine::CreateSpotLight()
{
	const auto& newLight = TMakeShared<PSSpotLight>();
	m_Lights.push_back(newLight);

	return newLight;
}

TShared<PModel> PGraphicsEngine::ImportModel(const PString& path)
{
	const auto& newModel = TMakeShared<PModel>();
	newModel->ImportModel(path, m_DefaultMaterial);
	m_Models.push_back(newModel);
	return newModel;
}

TShared<PSMaterial> PGraphicsEngine::CreateMaterial()
{
	return TMakeShared<PSMaterial>();
}

void PGraphicsEngine::CreateCollisionMesh(const TWeak<PSCollision>& col)
{
	if (const auto& colRef = col.lock())
	{
		TShared<PMesh> newMesh = TMakeShared<PMesh>();

		// Create a box of lines
		newMesh->CreateMesh(colMeshVData, colMeshIData);

		// Store the shared mesh into the collision
		colRef->debugMesh = newMesh;

		m_Collisions.push_back(col);
	}
}


/*
void PGraphicsEngine::ToggleFlashlight()
{
	if (m_Flashlight.lock()->intensity > 0)
		m_Flashlight.lock()->intensity = 0.0f;
	else
		m_Flashlight.lock()->intensity = 1.0f;
}

void PGraphicsEngine::IncreaseFlashlightRed()
{
	if (m_Flashlight.lock()->colour.r >= 1.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r + 0.1f,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::IncreaseFlashlightGreen()
{
	if (m_Flashlight.lock()->colour.r >= 1.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.g + 0.1f,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::IncreaseFlashlightBlue()
{
	if (m_Flashlight.lock()->colour.b >= 1.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b + 0.1f);
}

void PGraphicsEngine::DecreaseFlashlightRed()
{
	if (m_Flashlight.lock()->colour.r <= 0.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r - 0.1f,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::DecreaseFlashlightGreen()
{
	if (m_Flashlight.lock()->colour.g <= 0.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r,
		m_Flashlight.lock()->colour.g - 0.1f,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::DecreaseFlashlightBlue()
{
	if (m_Flashlight.lock()->colour.b <= 0.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b - 0.1f);
}

void PGraphicsEngine::IncreaseFlashlightInnerRadius()
{
	m_Flashlight.lock()->SetInnerCutOff(m_Flashlight.lock()->innerDegrees + 1);
}

void PGraphicsEngine::IncreaseFlashlightOuterRadius()
{
	m_Flashlight.lock()->SetOuterCutOff(m_Flashlight.lock()->outerDegrees + 1);
}

void PGraphicsEngine::DecreaseFlashlightInnerRadius()
{
	m_Flashlight.lock()->SetInnerCutOff(m_Flashlight.lock()->innerDegrees - 1);
}

void PGraphicsEngine::DecreaseFlashlightOuterRadius()
{
	m_Flashlight.lock()->SetOuterCutOff(m_Flashlight.lock()->outerDegrees - 1);
}

void PGraphicsEngine::IncreaseFlashlightIntensity()
{
	m_Flashlight.lock()->intensity += 0.1f;
}

void PGraphicsEngine::DecreaseFlashlightIntensity()
{
	m_Flashlight.lock()->intensity -= 0.1f;
}
*/