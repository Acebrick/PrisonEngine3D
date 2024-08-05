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
TWeak<PModel> m_Model;
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
	m_Camera->transform.position.z = -25.0f;

	// Create the texture object
	TShared<PTexture> defaultTexture = TMakeShared<PTexture>();

	// Add the texture to the mesh if it successfully created
	if (!defaultTexture->LoadTexture("Default Grid", "Textures/P_DefaultGrid.png"))
	{
		PDebug::Log("Graphics engine default texeture failed to load", LT_ERROR);
	}

	// DEBUG
	m_Model = ImportModel("Models/Axe/scene.gltf");

	TShared<PTexture> tex = TMakeShared<PTexture>();
	tex->LoadTexture("base colour", "Models/Axe/Textures/lambert2SG_baseColor.jpeg");
	TShared<PSMaterial> mat = TMakeShared<PSMaterial>();
	mat->m_BaseColourMap = tex;

	m_Model.lock()->SetMaterialBySlot(0, mat);

	// Making a second model
	//ImportModel("Models/Axe/scene.gltf").lock()->GetTransform().position = glm::vec3(0.0f, 15.0f, 0.0f);

	// Create the dir light
	const auto& dirLight = CreateDirLight();

	// Check if exists as a reference and change it
	if (const auto& lightRef = dirLight.lock())
	{
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.3f);
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

	//m_Model.lock()->GetTransform().rotation.x += 0.5f;
	//m_Model.lock()->GetTransform().rotation.y += 0.5f;
	//m_Model.lock()->GetTransform().rotation.z += 0.5f;

	//m_PointLight.lock()->position.z += 0.1f;

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
