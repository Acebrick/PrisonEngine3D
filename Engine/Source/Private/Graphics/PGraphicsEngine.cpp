#include "Graphics/PGraphicsEngine.h"
#include "Debug/PDebug.h"
#include "Graphics/PMesh.h"
#include "Graphics/PShaderProgram.h"
#include "Math/PSTransform.h"
#include "Graphics/PTexture.h"


// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

const std::vector<PSVertexData> vertexData = {
	//   x      y	   z      r     g     b       tx    ty
	{ {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 1 (top left)
	{ { 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 2 (top right)
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 3 (bot left) 
	{ { 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} } // vertex data 4 (bot right)
};

const std::vector<uint32_t> indexData = {
	 0, 1, 2, // Triangle 1
	 1, 2, 3  // Triangle 2
};

// Test mesh for debug
std::unique_ptr<PMesh> m_Mesh;

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

	m_Shader = std::make_shared<PShaderProgram>();

	// Attempt to initialise shdaer and test if failed
	if (!m_Shader->InitShader("Shaders/SimpleShader/SimpleShader.vertex", "Shaders/SimpleShader/SimpleShader.frag"))
	{
		PDebug::Log("Graphics engine failed to intialise due to shader failure");
		return false;
	}

	// Log the success of the graphics engine
	PDebug::Log("Successfully initialised graphics engine", LT_SUCCESS);

	// Create the debug mesh
	m_Mesh = std::make_unique<PMesh>();

	if (!m_Mesh->CreateMesh(vertexData, indexData))
	{
		PDebug::Log("Failed to create debug mesh");
	}

	// Create the texture object
	TShared<PTexture> defaultTexture = TMakeShared<PTexture>();

	// Add the texture to the mesh if it successfully created
	if (defaultTexture->LoadTexture("Default Grid", "Textures/P_DefaultGrid.png"))
	{
		m_Mesh->SetTexture(defaultTexture);
	}

	return true;
}

void PGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT);

	static PSTransform transform;
	//transform.rotation.z -= 0.5f;

	// Render custom graphics
	m_Mesh->Render(m_Shader, transform);

	// Presented the frame to the window
	// Swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}
