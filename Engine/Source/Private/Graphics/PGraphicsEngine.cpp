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
TUnique<PModel> m_Model;

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
	m_Camera->transform.position.z = -5.0f;

	// Create the texture object
	TShared<PTexture> defaultTexture = TMakeShared<PTexture>();

	// Add the texture to the mesh if it successfully created
	if (!defaultTexture->LoadTexture("Default Grid", "Textures/P_DefaultGrid.png"))
	{
		PDebug::Log("Graphics engine default texeture failed to load", LT_ERROR);
	}

	// DEBUG
	m_Model = TMakeUnique<PModel>();
	m_Model->ImportModel("Models/Axe/scene.gltf");
	m_Model->GetTransform().scale = glm::vec3(1.0f);
	m_Model->GetTransform().position.z = 100.0f;

	TShared<PSDirLight> dirLight = TMakeShared<PSDirLight>();
	dirLight->colour = glm::vec3(1.0f, 0.0f, 1.0f);
	dirLight->direction = glm::vec3(0.0f, -1.0f, 0.0f);
	m_Lights.push_back(dirLight);

	// Log the success of the graphics engine
	PDebug::Log("Successfully initialised graphics engine", LT_SUCCESS);

	return true;
}

void PGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background colour
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Activate the shader
	m_Shader->Activate();

	// Set the world transformations based on the camera
	m_Shader->SetWorldTransform(m_Camera);

	m_Model->GetTransform().rotation.x += 0.5f;
	m_Model->GetTransform().rotation.y += 0.5f;
	m_Model->GetTransform().rotation.z += 0.5f;

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	m_Model->Render(m_Shader, m_Lights);

	// Presented the frame to the window
	// Swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}