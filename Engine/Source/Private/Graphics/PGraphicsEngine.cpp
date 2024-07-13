#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PModel.h"
#include "Graphics/PShaderProgram.h"
#include "Math/PSTransform.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSCamera.h"

// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

// DEBUG
#include "PWindow.h"

// Test mesh for debug


TUnique<PModel> m_LTreads;
TUnique<PModel> m_Gun;
TUnique<PModel> m_Pillar;

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
	m_Body = TMakeShared<PModel>();
	m_Body->MakeCube(defaultTexture);
	m_Body->GetTransform().position.y = -3.0f;
	m_RTreads = TMakeShared<PModel>();
	m_RTreads->MakeRightTreads(defaultTexture);
	m_LTreads = TMakeUnique<PModel>();
	m_LTreads->MakeLeftTreads(defaultTexture);
	m_Gun = TMakeUnique<PModel>();
	m_Gun->MakePoly(defaultTexture);
	m_Pillar = TMakeUnique<PModel>();
	m_Pillar->MakePoly(defaultTexture);
	
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

	// Scale the gun
	m_Gun->GetTransform().scale = glm::vec3(0.8f);

	// Move tank components position as the body moves
	m_Gun->GetTransform().position = m_LTreads->GetTransform().position = m_RTreads->GetTransform().position = m_Body->GetTransform().position;

	// Rotate tank components as the body rotates
	m_RTreads->GetTransform().rotation = m_LTreads->GetTransform().rotation = m_Gun->GetTransform().rotation = m_Body->GetTransform().rotation;

	// Set the position of the pillars so you can see the tank moving
	m_Pillar->GetTransform().rotation.x = 90.0f;
	m_Pillar->GetTransform().position.x = 15.0f;

	// Activate the shader
	m_Shader->Activate();

	// Set the world transformations based on the camera
	m_Shader->SetWorldTransform(m_Camera);

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	m_Body->Render(m_Shader);
	m_RTreads->Render(m_Shader);
	m_LTreads->Render(m_Shader);
	m_Gun->Render(m_Shader);
	m_Pillar->Render(m_Shader);

	// Presented the frame to the window
	// Swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}
